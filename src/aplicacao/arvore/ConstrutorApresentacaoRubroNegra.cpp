#include "aplicacao/arvore/ConstrutorApresentacaoRubroNegra.hpp"

#include "aplicacao/arvore/DetalhesConstrucaoModelo.hpp"
#include "estruturas/ArvoreRubroNegra.hpp"

#include <memory>

namespace aplicacao::arvore {

apresentacao::arvore::ModeloVisualArvore
ConstrutorApresentacaoRubroNegra::construir(
    const std::vector<megasena::Registro>& registros) const
{
    const auto comparacoes = std::make_shared<std::size_t>(0);
    using Arvore = estruturas::ArvoreRubroNegra<
        megasena::Registro, detalhes::ComparadorRegistroContabilizado>;
    const auto resultado =
        detalhes::construirArvore<Arvore>(registros, comparacoes);
    const auto& arvore = resultado.arvore;
    return detalhes::criarModeloVisualComSemantica(
        arvore, resultado.comparacoes, arvore.quantidadeRotacoes(),
        [&arvore](auto visitante) {
            arvore.visitarNosComCorEmOrdem(
                [&visitante](const megasena::Registro& registro,
                             const megasena::Registro* pai,
                             bool filhoEsquerdo, std::size_t profundidade,
                             estruturas::CorNoRubroNegro cor) {
                    const auto semantica =
                        cor == estruturas::CorNoRubroNegro::Vermelho
                        ? apresentacao::arvore::SemanticaVisualNo::RubroNegroVermelho
                        : apresentacao::arvore::SemanticaVisualNo::RubroNegroPreto;
                    visitante(registro, pai, filhoEsquerdo, profundidade,
                              semantica);
                });
        },
        resultado.tempoInsercaoNanossegundos);
}

} // namespace aplicacao::arvore
