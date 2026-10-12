#include "interface/paginas/arvore/ItemNavegacaoEstrutura.hpp"

ItemNavegacaoEstrutura::ItemNavegacaoEstrutura(TipoEstrutura tipo, const QString& texto,
                                               const ParametrosComponente& parametros, QWidget* pai)
    : BotaoNavegacao(texto, parametros, pai), tipo_(tipo)
{
    setProperty("papelVisual", QStringLiteral("itemEstrutura"));
}

TipoEstrutura ItemNavegacaoEstrutura::tipo() const noexcept
{
    return tipo_;
}
