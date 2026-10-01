#pragma once

#include "interface/parametros/ParametrosComponente.hpp"

#include <QLabel>

class Texto : public QLabel
{
public:
    explicit Texto(const QString& conteudo, const ParametrosComponente& parametros = {}, QWidget* pai = nullptr);
    ~Texto() override = default;

    void definirConteudo(const QString& conteudo);
};
