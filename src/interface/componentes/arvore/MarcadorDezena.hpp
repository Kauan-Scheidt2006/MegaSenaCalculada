#pragma once

#include "interface/componentes/basicos/Texto.hpp"

class MarcadorDezena final : public Texto
{
    Q_OBJECT

public:
    explicit MarcadorDezena(int indice, QWidget* pai = nullptr);

    void apresentar(int dezena, bool correspondente);
    [[nodiscard]] int dezena() const noexcept;
    [[nodiscard]] bool correspondente() const noexcept;

private:
    int dezena_ = 0;
    bool correspondente_ = false;
};
