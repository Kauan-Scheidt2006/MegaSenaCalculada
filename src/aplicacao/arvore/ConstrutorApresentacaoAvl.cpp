#include "aplicacao/arvore/ConstrutorApresentacaoAvl.hpp"

#include "aplicacao/arvore/DetalhesConstrucaoModelo.hpp"
#include "estruturas/ArvoreAvl.hpp"

#include <memory>

namespace aplicacao::arvore {

apresentacao::arvore::ModeloVisualArvore
ConstrutorApresentacaoAvl::construir(
    const std::vector<megasena::Registro>& registros) const
{
    const auto comparacoes = std::make_shared<std::size_t>(0);
    using Arvore = estruturas::ArvoreAvl<
        megasena::Registro, detalhes::ComparadorRegistroContabilizado>;
    const auto resultado =
        detalhes::construirArvore<Arvore>(registros, comparacoes);
    return detalhes::criarModeloVisual(
        resultado.arvore, resultado.comparacoes,
        resultado.arvore.quantidadeRotacoes(),
        resultado.tempoInsercaoNanossegundos);
}

} // namespace aplicacao::arvore
