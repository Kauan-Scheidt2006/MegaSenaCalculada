#pragma once

#include <memory>
#include <utility>

namespace estruturas {

template<typename Valor, typename TipoNo>
struct NoArvoreBase {
    template<typename ValorRecebido>
    explicit NoArvoreBase(ValorRecebido&& novoValor)
        : valor(std::forward<ValorRecebido>(novoValor))
    {
    }

    Valor valor;
    std::unique_ptr<TipoNo> esquerdo;
    std::unique_ptr<TipoNo> direito;
};

} // namespace estruturas
