#pragma once

#include "interface/apresentacao/arvore/IdentificadorRegistro.hpp"

#include <QString>

#include <array>

namespace apresentacao::arvore {

struct DadosDetalhesRegistro {
    IdentificadorRegistro identificador;
    int numeroConcurso;
    QString dataConcurso;
    std::array<int, 6> dezenas;
};

} // namespace apresentacao::arvore
