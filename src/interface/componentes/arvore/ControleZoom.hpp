#pragma once

#include "interface/fundamentos/BaseContainer.hpp"

class Botao;
class Texto;

class ControleZoom final : public BaseContainer
{
    Q_OBJECT

public:
    explicit ControleZoom(QWidget* pai = nullptr);

    void definirPercentual(int percentual);

signals:
    void aumentoSolicitado();
    void reducaoSolicitada();

private:
    Botao* reduzir_ = nullptr;
    Texto* percentual_ = nullptr;
    Botao* aumentar_ = nullptr;
};
