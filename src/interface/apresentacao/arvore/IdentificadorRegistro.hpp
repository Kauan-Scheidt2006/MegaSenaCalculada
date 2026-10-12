#pragma once

#include <QMetaType>

#include <compare>

namespace apresentacao::arvore {

struct IdentificadorRegistro {
    int frequenciaAcumulada;
    int numero;

    auto operator<=>(const IdentificadorRegistro&) const = default;
};

} // namespace apresentacao::arvore

Q_DECLARE_METATYPE(apresentacao::arvore::IdentificadorRegistro)
