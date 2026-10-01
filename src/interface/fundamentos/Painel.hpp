#pragma once

#include "interface/fundamentos/BaseContainer.hpp"

/** Região visual delimitada que compõe uma página. */
class Painel : public BaseContainer
{
public:
    explicit Painel(const ParametrosContainer& parametros, QWidget* pai = nullptr);
    ~Painel() override = default;
};
