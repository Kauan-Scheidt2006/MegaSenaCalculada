#include "interface/fundamentos/BasePagina.hpp"

BasePagina::BasePagina(const ParametrosContainer& parametros, QWidget* pai)
    : BaseContainer(parametros, pai)
{
}

void BasePagina::ativar()
{
    show();
}

void BasePagina::desativar()
{
    hide();
}
