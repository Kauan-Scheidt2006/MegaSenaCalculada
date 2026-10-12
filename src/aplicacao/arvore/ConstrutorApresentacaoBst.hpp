#pragma once

#include "dominio/Registro.hpp"
#include "interface/apresentacao/arvore/ModeloVisualArvore.hpp"

#include <vector>

namespace aplicacao::arvore {

class ConstrutorApresentacaoBst final {
public:
    [[nodiscard]] apresentacao::arvore::ModeloVisualArvore construir(
        const std::vector<megasena::Registro>& registros) const;
};

} // namespace aplicacao::arvore
