#pragma once

#include "importacao/ConcursoImportado.hpp"

#include <filesystem>
#include <vector>

namespace megasena::infraestrutura {

class LeitorConcursosJson {
public:
    [[nodiscard]] std::vector<importacao::ConcursoImportado> ler(
        const std::filesystem::path& arquivo = "concursos.json") const;
};

} // namespace megasena::infraestrutura
