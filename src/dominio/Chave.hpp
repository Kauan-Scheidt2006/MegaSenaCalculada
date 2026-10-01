#pragma once

namespace megasena {

struct Chave {
    int frequenciaAcumulada;
    int numero;
};

struct ComparadorChave {
    [[nodiscard]] constexpr bool operator()(const Chave& esquerda,
                                            const Chave& direita) const noexcept 
    {
        if (esquerda.frequenciaAcumulada != direita.frequenciaAcumulada) {
            return esquerda.frequenciaAcumulada < direita.frequenciaAcumulada;
        }

        return esquerda.numero < direita.numero;
    }
};

[[nodiscard]] constexpr bool operator==(const Chave& esquerda,
                                        const Chave& direita) noexcept
{
    return esquerda.frequenciaAcumulada == direita.frequenciaAcumulada
        && esquerda.numero == direita.numero;
}

} // namespace megasena

