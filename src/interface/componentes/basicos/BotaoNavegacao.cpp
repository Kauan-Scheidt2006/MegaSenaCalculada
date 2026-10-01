#include "interface/componentes/basicos/BotaoNavegacao.hpp"

BotaoNavegacao::BotaoNavegacao(const QString& texto, const ParametrosComponente& parametros, QWidget* pai)
    : Botao(texto, parametros, pai)
{
    setCheckable(true);
    setAutoExclusive(true);
    setProperty("papelVisual", QStringLiteral("navegacao"));
}

void BotaoNavegacao::definirSelecionado(bool selecionado)
{
    setChecked(selecionado);
    setAccessibleDescription(selecionado ? QStringLiteral("Selecionado") : QStringLiteral("Nao selecionado"));
}

bool BotaoNavegacao::estaSelecionado() const noexcept
{
    return isChecked();
}
