#pragma once

#include "estruturas/NoArvoreBase.hpp"

#include <cstddef>
#include <utility>

namespace estruturas {

template<typename Valor>
struct NoAvl final : NoArvoreBase<Valor, NoAvl<Valor>> {
    using Base = NoArvoreBase<Valor, NoAvl<Valor>>;

    template<typename ValorRecebido>
    explicit NoAvl(ValorRecebido&& novoValor)
        : Base(std::forward<ValorRecebido>(novoValor))
    {
    }

    std::size_t altura = 1;
};

} // namespace estruturas
