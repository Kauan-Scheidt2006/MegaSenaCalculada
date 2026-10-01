#pragma once

#include "interface/fundamentos/BaseContainer.hpp"

class AreaPaginas;
class BarraEstado;

class EstruturaAmbiente final : public BaseContainer
{
public:
    explicit EstruturaAmbiente(QWidget* pai = nullptr);

    [[nodiscard]] AreaPaginas& areaPaginas() const noexcept;
    [[nodiscard]] BarraEstado& barraEstado() const noexcept;

private:
    AreaPaginas* areaPaginas_ = nullptr;
    BarraEstado* barraEstado_ = nullptr;
};
