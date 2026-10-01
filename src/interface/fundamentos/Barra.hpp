#pragma once

#include "interface/fundamentos/BaseContainer.hpp"

/** Container linear para ações ou informações relacionadas. */
class Barra : public BaseContainer
{
public:
    explicit Barra(const ParametrosContainer& parametros, QWidget* pai = nullptr);
    ~Barra() override = default;
};
