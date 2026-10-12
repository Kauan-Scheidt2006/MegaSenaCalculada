#pragma once

#include <cstddef>
#include <compare>

namespace megasena {

struct AmostraAlturaArvore {
    std::size_t quantidadeInserida = 0;
    std::size_t altura = 0;

    auto operator<=>(const AmostraAlturaArvore&) const = default;
};

} // namespace megasena
