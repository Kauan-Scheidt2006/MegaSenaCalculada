#pragma once

#include "interface/fundamentos/BaseContainer.hpp"

class BasePagina;

class AreaPaginas final : public BaseContainer
{
public:
    explicit AreaPaginas(QWidget* pai = nullptr);

    void adicionarPagina(BasePagina& pagina);
    void exibirPagina(BasePagina& pagina);
};
