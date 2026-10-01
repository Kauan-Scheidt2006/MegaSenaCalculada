#pragma once

#include "interface/fundamentos/BaseContainer.hpp"

/** Base dos componentes que representam destinos de navegação da interface. */
class BasePagina : public BaseContainer
{
public:
    explicit BasePagina(const ParametrosContainer& parametros, QWidget* pai = nullptr);
    ~BasePagina() override = default;

    virtual void ativar();
    virtual void desativar();
};
