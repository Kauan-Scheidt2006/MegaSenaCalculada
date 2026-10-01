#pragma once

#include "interface/parametros/ParametrosComponente.hpp"
#include "interface/parametros/TipoDisposicao.hpp"

#include <QMargins>

#include <optional>

struct ParametrosContainer
{
    ParametrosComponente componente;
    TipoDisposicao disposicao = TipoDisposicao::Vertical;
    QMargins margens{0, 0, 0, 0};
    std::optional<int> espacamento;
    std::optional<Qt::Alignment> alinhamento;
};
