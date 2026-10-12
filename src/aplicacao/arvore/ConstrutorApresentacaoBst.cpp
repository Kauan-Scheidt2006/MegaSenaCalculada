#include "aplicacao/arvore/ConstrutorApresentacaoBst.hpp"

#include "aplicacao/arvore/DetalhesConstrucaoModelo.hpp"
#include "estruturas/ArvoreBinariaBusca.hpp"

#include <memory>

namespace aplicacao::arvore {

apresentacao::arvore::ModeloVisualArvore ConstrutorApresentacaoBst::construir(
    const std::vector<megasena::Registro>& registros) const
{
    const auto comparacoes = std::make_shared<std::size_t>(0);
    using Arvore = estruturas::ArvoreBinariaBusca<
        megasena::Registro, detalhes::ComparadorRegistroContabilizado>;
    const auto resultado =
        detalhes::construirArvore<Arvore>(registros, comparacoes);
    return detalhes::criarModeloVisual(
        resultado.arvore, resultado.comparacoes, 0,
        resultado.tempoInsercaoNanossegundos);
}

} // namespace aplicacao::arvore
