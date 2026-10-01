#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace megasena::importacao {

struct ConcursoImportado {
    int numeroConcurso;
    std::string data;
    std::vector<int> dezenas;
    std::size_t indiceOrigem;
};

} // namespace megasena::importacao
