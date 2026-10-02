#pragma once

#include "interface/componentes/basicos/Texto.hpp"

class Subtitulo final : public Texto
{
public:
    explicit Subtitulo(const QString& conteudo, ParametrosComponente parametros, QWidget* pai = nullptr);
};
