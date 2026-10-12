#pragma once

#include "interface/componentes/basicos/BotaoNavegacao.hpp"
#include "interface/paginas/arvore/TipoEstrutura.hpp"

/** Item acionavel que representa somente a intencao de escolher uma estrutura. */
class ItemNavegacaoEstrutura final : public BotaoNavegacao
{
public:
    ItemNavegacaoEstrutura(TipoEstrutura tipo, const QString& texto,
                           const ParametrosComponente& parametros, QWidget* pai = nullptr);

    [[nodiscard]] TipoEstrutura tipo() const noexcept;

private:
    TipoEstrutura tipo_;
};
