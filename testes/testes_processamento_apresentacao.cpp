#include "aplicacao/arvore/ConstrutorApresentacaoAvl.hpp"
#include "aplicacao/arvore/ConstrutorApresentacaoBst.hpp"
#include "aplicacao/arvore/ConstrutorApresentacaoRubroNegra.hpp"
#include "aplicacao/arvore/ConsultorDetalhesRegistro.hpp"
#include "aplicacao/arvore/ExecutorPercursosArvore.hpp"
#include "aplicacao/arvore/SessaoConstrucaoAvlPassoAPasso.hpp"
#include "infraestrutura/json/LeitorConcursosJson.hpp"
#include "processamento/ProcessadorHistorico.hpp"

#include <algorithm>
#include <cassert>
#include <filesystem>
#include <stdexcept>
#include <vector>

using megasena::importacao::ConcursoImportado;

namespace {

void testarOrdemFisicaDoJson()
{
    const std::vector<ConcursoImportado> concursos{
        {20, "02/01/2026", {30, 10, 20, 40, 50, 60}, 0},
        {10, "01/01/2026", {20, 1, 2, 3, 4, 5}, 1}};
    const auto registros =
        megasena::processamento::ProcessadorHistorico{}.processar(concursos);

    assert(registros.size() == 12);
    assert(registros[0].numeroConcurso == 20);
    assert(registros[0].chave.numero == 30);
    assert(registros[1].chave.numero == 10);
    assert(registros[6].numeroConcurso == 10);
    assert(registros[6].chave.numero == 20);
    assert(registros[6].chave.frequenciaAcumulada == 2);
}

void testarEstadoFinalDaBst()
{
    const std::vector<megasena::Registro> registros{
        {{1, 30}, 1}, {{1, 10}, 1}, {{1, 20}, 1}, {{1, 40}, 1}};
    const auto modelo =
        aplicacao::arvore::ConstrutorApresentacaoBst{}.construir(registros);

    assert(modelo.metricas().quantidadeNos == 4);
    assert(modelo.metricas().altura == 3);
    assert(modelo.metricas().comparacoes > 0);
    assert(modelo.metricas().rotacoes == 0);
    assert(modelo.metricas().tempoInsercaoNanossegundos > 0);
    assert(modelo.metricas().metricasPeriodicasCalculadas);
    assert(modelo.metricas().alturasPeriodicas.empty());
    assert(modelo.nos().size() == 4);
    assert(modelo.raiz().has_value());
    assert(modelo.raiz()->numero == 30);
    assert(modelo.raiz()->frequenciaAcumulada == 1);
}

void testarEstadoFinalDaAvlComMesmaSequencia()
{
    const std::vector<megasena::Registro> registros{
        {{1, 10}, 1}, {{1, 20}, 1}, {{1, 30}, 1}, {{1, 40}, 1}};
    const auto bst =
        aplicacao::arvore::ConstrutorApresentacaoBst{}.construir(registros);
    const auto avl =
        aplicacao::arvore::ConstrutorApresentacaoAvl{}.construir(registros);

    assert(bst.nos().size() == registros.size());
    assert(avl.nos().size() == registros.size());
    assert(avl.metricas().quantidadeNos == registros.size());
    assert(avl.metricas().altura < bst.metricas().altura);
    assert(avl.metricas().comparacoes > 0);
    assert(avl.metricas().rotacoes > 0);
    assert(avl.metricas().tempoInsercaoNanossegundos > 0);
    assert(avl.metricas().metricasPeriodicasCalculadas);
    assert(avl.raiz().has_value());
    assert(avl.raiz()->numero == 20);
}

void testarEstadoFinalDaRubroNegra()
{
    using apresentacao::arvore::IdentificadorRegistro;
    using apresentacao::arvore::LadoLigacaoArvore;
    using apresentacao::arvore::SemanticaVisualNo;

    const std::vector<megasena::Registro> registros{
        {{1, 30}, 1}, {{1, 20}, 1}, {{1, 10}, 1}};
    const auto modelo =
        aplicacao::arvore::ConstrutorApresentacaoRubroNegra{}.construir(
            registros);

    assert(modelo.metricas().quantidadeNos == 3);
    assert(modelo.metricas().altura == 2);
    assert(modelo.metricas().comparacoes == 3);
    assert(modelo.metricas().rotacoes == 1);
    assert(modelo.metricas().tempoInsercaoNanossegundos > 0);
    assert(modelo.metricas().metricasPeriodicasCalculadas);
    assert((modelo.raiz() == IdentificadorRegistro{1, 20}));
    assert(modelo.nos().size() == 3);
    assert((modelo.nos()[0].identificador == IdentificadorRegistro{1, 10}));
    assert((modelo.nos()[1].identificador == IdentificadorRegistro{1, 20}));
    assert((modelo.nos()[2].identificador == IdentificadorRegistro{1, 30}));
    assert(modelo.nos()[0].semanticaVisual
           == SemanticaVisualNo::RubroNegroVermelho);
    assert(modelo.nos()[1].semanticaVisual
           == SemanticaVisualNo::RubroNegroPreto);
    assert(modelo.nos()[2].semanticaVisual
           == SemanticaVisualNo::RubroNegroVermelho);
    assert(modelo.ligacoes().size() == 2);

    const auto& ligacaoEsquerda = modelo.ligacoes()[0];
    assert((ligacaoEsquerda.pai == IdentificadorRegistro{1, 20}));
    assert((ligacaoEsquerda.filho == IdentificadorRegistro{1, 10}));
    assert(ligacaoEsquerda.lado == LadoLigacaoArvore::Esquerdo);

    const auto& ligacaoDireita = modelo.ligacoes()[1];
    assert((ligacaoDireita.pai == IdentificadorRegistro{1, 20}));
    assert((ligacaoDireita.filho == IdentificadorRegistro{1, 30}));
    assert(ligacaoDireita.lado == LadoLigacaoArvore::Direito);

    const auto modeloComNoVermelho =
        aplicacao::arvore::ConstrutorApresentacaoRubroNegra{}.construir(
            {{{1, 10}, 1}, {{1, 20}, 1}});
    assert(modeloComNoVermelho.nos()[0].semanticaVisual
           == SemanticaVisualNo::RubroNegroPreto);
    assert(modeloComNoVermelho.nos()[1].semanticaVisual
           == SemanticaVisualNo::RubroNegroVermelho);
}

void testarExecucaoDosPercursosNasArvores()
{
    using aplicacao::arvore::ExecutorPercursosArvore;
    using aplicacao::arvore::TipoPercursoArvore;

    const std::vector<megasena::Registro> registros{
        {{1, 20}, 1}, {{1, 10}, 1}, {{1, 30}, 1}};
    ExecutorPercursosArvore executor(registros);

    for (TipoEstrutura estrutura : {TipoEstrutura::BuscaBinaria,
                                    TipoEstrutura::Avl,
                                    TipoEstrutura::RubroNegra}) {
        const auto emOrdem = executor.executar(
            estrutura, TipoPercursoArvore::EmOrdem);
        const auto preOrdem = executor.executar(
            estrutura, TipoPercursoArvore::PreOrdem);
        const auto posOrdem = executor.executar(
            estrutura, TipoPercursoArvore::PosOrdem);

        assert(emOrdem.quantidadeVisitada == 3);
        assert(preOrdem.quantidadeVisitada == 3);
        assert(posOrdem.quantidadeVisitada == 3);
        assert(emOrdem.registros[0].chave.numero == 10);
        assert(emOrdem.registros[1].chave.numero == 20);
        assert(emOrdem.registros[2].chave.numero == 30);
        assert(preOrdem.registros[0].chave.numero == 20);
        assert(posOrdem.registros[2].chave.numero == 20);
        const auto encontrada = executor.buscar(estrutura, {1, 10});
        assert(encontrada.registro.has_value());
        assert(encontrada.registro->chave.numero == 10);
        assert(encontrada.nosVisitados == 2);
        assert(encontrada.comparacoes == 3);
        const auto ausente = executor.buscar(estrutura, {1, 60});
        assert(!ausente.registro.has_value());
        assert(ausente.nosVisitados == 2);
        assert(ausente.comparacoes == 4);
        const auto remocao = executor.remover(estrutura, {1, 10});
        assert(remocao.removido);
        assert(remocao.nosVisitados == 2);
        assert(remocao.comparacoes == 3);
        assert(!executor.buscar(estrutura, {1, 10}).registro.has_value());
        const auto modeloAtualizado = executor.criarModeloVisual(estrutura);
        assert(modeloAtualizado.nos().size() == 2);
        assert(modeloAtualizado.buscarNo({1, 10}) == nullptr);
        assert(modeloAtualizado.metricas().quantidadeNos == 2);
        const auto repetida = executor.remover(estrutura, {1, 10});
        assert(!repetida.removido);
        assert(repetida.nosVisitados > 0);
        assert(repetida.comparacoes > 0);
    }
}

void testarBuscaUsaChaveCompleta()
{
    const std::vector<megasena::Registro> registros{
        {{3, 10}, 100}, {{3, 27}, 200}, {{4, 10}, 300}};
    const aplicacao::arvore::ExecutorPercursosArvore executor(registros);

    for (TipoEstrutura estrutura : {TipoEstrutura::BuscaBinaria,
                                    TipoEstrutura::Avl,
                                    TipoEstrutura::RubroNegra}) {
        const auto numeroDez = executor.buscar(estrutura, {3, 10});
        const auto numeroVinteESete = executor.buscar(estrutura, {3, 27});
        const auto outraFrequencia = executor.buscar(estrutura, {4, 10});

        assert(numeroDez.registro.has_value());
        assert(numeroDez.registro->numeroConcurso == 100);
        assert(numeroVinteESete.registro.has_value());
        assert(numeroVinteESete.registro->numeroConcurso == 200);
        assert(outraFrequencia.registro.has_value());
        assert(outraFrequencia.registro->numeroConcurso == 300);
        assert(!executor.buscar(estrutura, {3, 11}).registro.has_value());
    }
}

void testarModelosMantemMesmaOrdemDeRegistros()
{
    const std::vector<megasena::Registro> registros{
        {{2, 30}, 1}, {{1, 40}, 1}, {{2, 10}, 1}, {{1, 20}, 1}};
    const auto bst =
        aplicacao::arvore::ConstrutorApresentacaoBst{}.construir(registros);
    const auto avl =
        aplicacao::arvore::ConstrutorApresentacaoAvl{}.construir(registros);
    const auto rubroNegra =
        aplicacao::arvore::ConstrutorApresentacaoRubroNegra{}.construir(
            registros);

    assert(bst.nos().size() == registros.size());
    assert(avl.nos().size() == registros.size());
    assert(rubroNegra.nos().size() == registros.size());
    for (std::size_t indice = 0; indice < registros.size(); ++indice) {
        assert(rubroNegra.nos()[indice].identificador
               == bst.nos()[indice].identificador);
        assert(rubroNegra.nos()[indice].identificador
               == avl.nos()[indice].identificador);
        assert(bst.nos()[indice].semanticaVisual
               == apresentacao::arvore::SemanticaVisualNo::Padrao);
        assert(avl.nos()[indice].semanticaVisual
               == apresentacao::arvore::SemanticaVisualNo::Padrao);
    }
}

void testarConsultaDeDetalhesDoConcursoCorreto()
{
    const std::vector<ConcursoImportado> concursos{
        {20, "02/01/2026", {30, 10, 20, 40, 50, 60}, 0},
        {21, "03/01/2026", {20, 1, 2, 3, 4, 5}, 1}};
    const auto registros =
        megasena::processamento::ProcessadorHistorico{}.processar(concursos);
    const aplicacao::arvore::ConsultorDetalhesRegistro consultor{
        registros, concursos};

    const auto* primeiraOcorrencia = consultor.buscar({1, 20});
    const auto* segundaOcorrencia = consultor.buscar({2, 20});
    assert(primeiraOcorrencia != nullptr);
    assert(primeiraOcorrencia->numeroConcurso == 20);
    assert(primeiraOcorrencia->dataConcurso == QStringLiteral("02/01/2026"));
    const std::array<int, 6> dezenasEsperadas{30, 10, 20, 40, 50, 60};
    assert(primeiraOcorrencia->dezenas == dezenasEsperadas);
    assert(segundaOcorrencia != nullptr);
    assert(segundaOcorrencia->numeroConcurso == 21);
    assert(segundaOcorrencia->dataConcurso == QStringLiteral("03/01/2026"));
    assert(consultor.buscar({99, 20}) == nullptr);
}

void testarSessaoAvlPassoAPasso()
{
    const std::vector<megasena::Registro> registros{
        {{1, 30}, 1}, {{1, 10}, 1}, {{1, 20}, 1}};
    aplicacao::arvore::SessaoConstrucaoAvlPassoAPasso sessao(registros);
    const auto inicial = sessao.estadoInicial();
    assert(inicial.modelo.vazia());
    assert(inicial.numeroPasso == 0);
    assert(!inicial.concluido);

    aplicacao::arvore::ResultadoPassoAvl resultado = inicial;
    bool descreveuChamadaRecursiva = false;
    bool descreveuRetorno = false;
    bool descreveuRotacaoComRetorno = false;
    std::size_t limite = 100;
    while (!resultado.concluido && limite-- > 0) {
        resultado = sessao.avancar();
        assert(!resultado.descricao.contains(QStringLiteral("dezena 0")));
        descreveuChamadaRecursiva = descreveuChamadaRecursiva
            || resultado.descricao.contains(
                QStringLiteral("inserirNaSubarvore(no->esquerdo, valor)"));
        descreveuRetorno = descreveuRetorno
            || resultado.descricao.contains(
                QStringLiteral("Retorno da chamada que recebeu"));
        descreveuRotacaoComRetorno = descreveuRotacaoComRetorno
            || (resultado.descricao.contains(QStringLiteral("rebalancear"))
                && resultado.descricao.contains(
                    QStringLiteral("A função retornaria a nova raiz")));
    }
    assert(limite > 0);
    assert(resultado.concluido);
    assert(descreveuChamadaRecursiva);
    assert(descreveuRetorno);
    assert(descreveuRotacaoComRetorno);
    assert(resultado.descricao.contains(
        QStringLiteral("A inserção de 20 retornou até a chamada inicial")));
    assert(resultado.descricao.contains(
        QStringLiteral("construção da AVL foi concluída")));
    assert(resultado.modelo.metricas().quantidadeNos == 3);
    assert(resultado.modelo.metricas().altura == 2);
    assert(resultado.modelo.metricas().rotacoes == 2);
    assert(resultado.modelo.raiz().has_value());
    assert(resultado.modelo.raiz()->numero == 20);

    sessao.reiniciar();
    assert(sessao.estadoInicial().modelo.vazia());
}

void testarValidacaoAntesDoProcessamento()
{
    bool falhou = false;
    try {
        (void)megasena::processamento::ProcessadorHistorico{}.processar(
            {{1, "01/01/2026", {1, 2, 3, 4, 5, 5}, 0}});
    } catch (const std::invalid_argument&) {
        falhou = true;
    }
    assert(falhou);

    falhou = false;
    try {
        (void)megasena::processamento::ProcessadorHistorico{}.processar(
            {{1, "01/01/2026", {1, 2, 3, 4, 5, 6}, 0},
             {1, "02/01/2026", {7, 8, 9, 10, 11, 12}, 1}});
    } catch (const std::invalid_argument&) {
        falhou = true;
    }
    assert(falhou);
}

void testarAssociacoesInvalidas()
{
    bool falhou = false;
    try {
        const aplicacao::arvore::ConsultorDetalhesRegistro consultor{
            {{{1, 7}, 999}},
            {{1, "01/01/2026", {1, 2, 3, 4, 5, 6}, 0}}};
        (void)consultor;
    } catch (const std::invalid_argument&) {
        falhou = true;
    }
    assert(falhou);

    falhou = false;
    try {
        const aplicacao::arvore::ConsultorDetalhesRegistro consultor{
            {{{1, 7}, 1}},
            {{1, "01/01/2026", {1, 2, 3, 4, 5, 6}, 0}}};
        (void)consultor;
    } catch (const std::invalid_argument&) {
        falhou = true;
    }
    assert(falhou);
}

void testarArquivoRealDoProjeto()
{
    const auto concursos = megasena::infraestrutura::LeitorConcursosJson{}.ler(
        std::filesystem::path(MEGASENA_DIRETORIO_FONTE) / "concursos.json");
    const auto registros =
        megasena::processamento::ProcessadorHistorico{}.processar(concursos);
    const auto modelo =
        aplicacao::arvore::ConstrutorApresentacaoBst{}.construir(registros);

    assert(!registros.empty());
    assert(modelo.metricas().quantidadeNos == registros.size());
    assert(modelo.metricas().altura > 0);
    assert(modelo.raiz().has_value());
    assert(modelo.raiz()->numero == registros.front().chave.numero);
    assert(modelo.raiz()->frequenciaAcumulada
           == registros.front().chave.frequenciaAcumulada);
}

} // namespace

int main(int, char**)
{
    testarOrdemFisicaDoJson();
    testarEstadoFinalDaBst();
    testarEstadoFinalDaAvlComMesmaSequencia();
    testarEstadoFinalDaRubroNegra();
    testarExecucaoDosPercursosNasArvores();
    testarBuscaUsaChaveCompleta();
    testarModelosMantemMesmaOrdemDeRegistros();
    testarConsultaDeDetalhesDoConcursoCorreto();
    testarSessaoAvlPassoAPasso();
    testarValidacaoAntesDoProcessamento();
    testarAssociacoesInvalidas();
    testarArquivoRealDoProjeto();
    return 0;
}
