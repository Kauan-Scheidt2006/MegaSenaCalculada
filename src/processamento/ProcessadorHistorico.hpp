#pragma once

#include "dominio/Registro.hpp"
#include "importacao/ConcursoImportado.hpp"

#include <vector>

namespace megasena::processamento {

class ProcessadorHistorico final {
public:
    [[nodiscard]] std::vector<Registro> processar(
        const std::vector<importacao::ConcursoImportado>& concursos) const;
};

} // namespace megasena::processamento
