#pragma once

#include "interface/parametros/ParametrosComponente.hpp"

#include <QProgressBar>

class IndicadorProgresso final : public QProgressBar
{
public:
    explicit IndicadorProgresso(const ParametrosComponente& parametros = {}, QWidget* pai = nullptr);

    void definirProgresso(int atual, int total);
    void definirIndeterminado();
};
