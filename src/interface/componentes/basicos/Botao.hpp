#pragma once

#include "interface/parametros/ParametrosComponente.hpp"

#include <QPushButton>

class Botao : public QPushButton
{
public:
    explicit Botao(const QString& texto, const ParametrosComponente& parametros, QWidget* pai = nullptr);
    ~Botao() override = default;

    void definirTexto(const QString& texto);
};
