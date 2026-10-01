#pragma once

#include "interface/fundamentos/Barra.hpp"

class Texto;

class BarraEstado final : public Barra
{
public:
    explicit BarraEstado(QWidget* pai = nullptr);

    void definirMensagem(const QString& mensagem);
    [[nodiscard]] QString mensagem() const;

private:
    Texto* mensagem_ = nullptr;
};
