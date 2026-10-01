#pragma once

#include "interface/componentes/basicos/Botao.hpp"

class BotaoIcone final : public Botao
{
public:
    explicit BotaoIcone(const QString& recursoIcone, const QString& textoAlternativo,
                        const ParametrosComponente& parametros = {}, QWidget* pai = nullptr);

    void definirIcone(const QString& recursoIcone);
};
