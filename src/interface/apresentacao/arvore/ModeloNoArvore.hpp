#pragma once

#include "interface/apresentacao/arvore/IdentificadorRegistro.hpp"

namespace apresentacao::arvore {

enum class SemanticaVisualNo {
    Padrao,
    RubroNegroPreto,
    RubroNegroVermelho,
};

struct ModeloNoArvore {
    IdentificadorRegistro identificador;
    SemanticaVisualNo semanticaVisual = SemanticaVisualNo::Padrao;
};

} // namespace apresentacao::arvore
