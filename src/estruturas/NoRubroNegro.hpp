#pragma once

#include "estruturas/NoArvoreBase.hpp"

#include <utility>

namespace estruturas {

enum class CorNoRubroNegro {
    Vermelho,
    Preto,
};

template<typename Valor>
struct NoRubroNegro final
    : NoArvoreBase<Valor, NoRubroNegro<Valor>> {
    using Base = NoArvoreBase<Valor, NoRubroNegro<Valor>>;

    template<typename ValorRecebido>
    explicit NoRubroNegro(ValorRecebido&& novoValor)
        : Base(std::forward<ValorRecebido>(novoValor))
    {
    }

    CorNoRubroNegro cor = CorNoRubroNegro::Vermelho;
    NoRubroNegro* pai = nullptr;
};

} // namespace estruturas
