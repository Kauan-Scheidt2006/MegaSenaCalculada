#pragma once

#include "interface/componentes/basicos/Botao.hpp"

class BotaoNavegacao final : public Botao
{
public:
    explicit BotaoNavegacao(const QString& texto, const ParametrosComponente& parametros = {},
                            QWidget* pai = nullptr);

    void definirSelecionado(bool selecionado);
    [[nodiscard]] bool estaSelecionado() const noexcept;
};
