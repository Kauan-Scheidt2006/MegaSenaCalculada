#pragma once

#include "interface/apresentacao/arvore/IdentificadorRegistro.hpp"

namespace apresentacao::arvore {

enum class LadoLigacaoArvore {
    Esquerdo,
    Direito,
};

struct ModeloLigacaoArvore {
    IdentificadorRegistro pai;
    IdentificadorRegistro filho;
    LadoLigacaoArvore lado;
};

} // namespace apresentacao::arvore
