#pragma once

#include "interface/componentes/basicos/Texto.hpp"

class Titulo final : public Texto
{
public:
    explicit Titulo(const QString& conteudo, ParametrosComponente parametros = {}, QWidget* pai = nullptr);
};
