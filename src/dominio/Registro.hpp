#pragma once

#include "dominio/Chave.hpp"

namespace megasena {

struct Registro {
    Chave chave;
    int numeroConcurso;
};

struct ComparadorRegistro {
    [[nodiscard]] constexpr bool operator()(const Registro& esquerda,
                                            const Registro& direita) const noexcept
    {
        return ComparadorChave{}(esquerda.chave, direita.chave);
    }
};

} // namespace megasena
