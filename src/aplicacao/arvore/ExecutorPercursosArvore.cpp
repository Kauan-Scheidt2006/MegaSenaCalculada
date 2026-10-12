#include "aplicacao/arvore/ExecutorPercursosArvore.hpp"

#include "aplicacao/arvore/DetalhesConstrucaoModelo.hpp"

#include <chrono>
#include <stdexcept>

namespace aplicacao::arvore {
namespace {

// Comente a linha abaixo para desativar somente a medicao de tempo da remocao.
#define MEDIR_TEMPO_REMOCAO

template<typename Arvore>
std::vector<megasena::Registro> executarPercurso(
    const Arvore& arvore, TipoPercursoArvore percurso)
{
    switch (percurso) {
    case TipoPercursoArvore::EmOrdem:
        return arvore.percorrerEmOrdem();
    case TipoPercursoArvore::PreOrdem:
        return arvore.percorrerPreOrdem();
    case TipoPercursoArvore::PosOrdem:
        return arvore.percorrerPosOrdem();
    }
    throw std::invalid_argument("Tipo de percurso desconhecido.");
}

template<typename Arvore>
ResultadoPercursoArvore medirPercurso(
    const Arvore& arvore, TipoPercursoArvore percurso)
{
    const auto inicio = std::chrono::steady_clock::now();
    auto registros = executarPercurso(arvore, percurso);
    const auto fim = std::chrono::steady_clock::now();
    const auto duracao = std::chrono::duration_cast<std::chrono::nanoseconds>(
        fim - inicio);
    const auto quantidadeVisitada = registros.size();
    return {std::move(registros), quantidadeVisitada,
            static_cast<std::uint64_t>(duracao.count())};
}

template<typename Arvore>
ResultadoBuscaArvore medirBusca(
    const Arvore& arvore, const megasena::Chave& chave)
{
    estruturas::MetricasNavegacaoArvore metricas;
    const megasena::Registro consulta{chave, 0};
    const auto inicio = std::chrono::steady_clock::now();
    const auto* encontrado = arvore.buscar(consulta, metricas);
    const auto fim = std::chrono::steady_clock::now();
    const auto duracao = std::chrono::duration_cast<std::chrono::nanoseconds>(
        fim - inicio);
    return {encontrado ? std::optional<megasena::Registro>(*encontrado)
                       : std::nullopt,
            metricas.nosVisitados, metricas.comparacoes,
            static_cast<std::uint64_t>(duracao.count())};
}

template<typename Arvore>
ResultadoRemocaoArvore medirRemocao(
    Arvore& arvore, const megasena::Chave& chave)
{
    estruturas::MetricasNavegacaoArvore metricas;
    const megasena::Registro consulta{chave, 0};
#ifdef MEDIR_TEMPO_REMOCAO
    const auto inicio = std::chrono::steady_clock::now();
#endif
    const bool removido = arvore.remover(consulta, metricas);
#ifdef MEDIR_TEMPO_REMOCAO
    const auto fim = std::chrono::steady_clock::now();
    const auto duracao = std::chrono::duration_cast<std::chrono::nanoseconds>(
        fim - inicio);
    return {removido, metricas.nosVisitados, metricas.comparacoes,
            static_cast<std::uint64_t>(duracao.count())};
#else
    return {removido, metricas.nosVisitados, metricas.comparacoes, 0};
#endif
}

} // namespace

ExecutorPercursosArvore::ExecutorPercursosArvore(
    const std::vector<megasena::Registro>& registros)
{
    for (const auto& registro : registros) {
        if (!bst_.inserir(registro) || !avl_.inserir(registro)
            || !rubroNegra_.inserir(registro)) {
            throw std::invalid_argument(
                "A sequencia historica produziu uma chave duplicada.");
        }
    }
}

ResultadoPercursoArvore ExecutorPercursosArvore::executar(
    TipoEstrutura estrutura, TipoPercursoArvore percurso) const
{
    switch (estrutura) {
    case TipoEstrutura::BuscaBinaria:
        return medirPercurso(bst_, percurso);
    case TipoEstrutura::Avl:
    case TipoEstrutura::AvlPassoAPasso:
        return medirPercurso(avl_, percurso);
    case TipoEstrutura::RubroNegra:
        return medirPercurso(rubroNegra_, percurso);
    }
    throw std::invalid_argument("Tipo de estrutura desconhecido.");
}

ResultadoBuscaArvore ExecutorPercursosArvore::buscar(
    TipoEstrutura estrutura, const megasena::Chave& chave) const
{
    switch (estrutura) {
    case TipoEstrutura::BuscaBinaria:
        return medirBusca(bst_, chave);
    case TipoEstrutura::Avl:
    case TipoEstrutura::AvlPassoAPasso:
        return medirBusca(avl_, chave);
    case TipoEstrutura::RubroNegra:
        return medirBusca(rubroNegra_, chave);
    }
    throw std::invalid_argument("Tipo de estrutura desconhecido.");
}

ResultadoRemocaoArvore ExecutorPercursosArvore::remover(
    TipoEstrutura estrutura, const megasena::Chave& chave)
{
    switch (estrutura) {
    case TipoEstrutura::BuscaBinaria:
        return medirRemocao(bst_, chave);
    case TipoEstrutura::Avl:
    case TipoEstrutura::AvlPassoAPasso:
        return medirRemocao(avl_, chave);
    case TipoEstrutura::RubroNegra:
        return medirRemocao(rubroNegra_, chave);
    }
    throw std::invalid_argument("Tipo de estrutura desconhecido.");
}

apresentacao::arvore::ModeloVisualArvore
ExecutorPercursosArvore::criarModeloVisual(TipoEstrutura estrutura) const
{
    switch (estrutura) {
    case TipoEstrutura::BuscaBinaria:
        return detalhes::criarModeloVisual(bst_, 0, 0);
    case TipoEstrutura::Avl:
    case TipoEstrutura::AvlPassoAPasso:
        return detalhes::criarModeloVisual(
            avl_, 0, avl_.quantidadeRotacoes());
    case TipoEstrutura::RubroNegra:
        return detalhes::criarModeloVisualComSemantica(
            rubroNegra_, 0, rubroNegra_.quantidadeRotacoes(),
            [this](auto visitante) {
                rubroNegra_.visitarNosComCorEmOrdem(
                    [&visitante](const megasena::Registro& registro,
                                 const megasena::Registro* pai,
                                 bool filhoEsquerdo,
                                 std::size_t profundidade,
                                 estruturas::CorNoRubroNegro cor) {
                        const auto semantica =
                            cor == estruturas::CorNoRubroNegro::Vermelho
                            ? apresentacao::arvore::SemanticaVisualNo::RubroNegroVermelho
                            : apresentacao::arvore::SemanticaVisualNo::RubroNegroPreto;
                        visitante(registro, pai, filhoEsquerdo, profundidade,
                                  semantica);
                    });
            });
    }
    throw std::invalid_argument("Tipo de estrutura desconhecido.");
}

} // namespace aplicacao::arvore
