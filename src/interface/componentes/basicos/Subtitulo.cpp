#include "interface/componentes/basicos/Subtitulo.hpp"

Subtitulo::Subtitulo(const QString& conteudo, ParametrosComponente parametros, QWidget* pai)
    : Texto(conteudo, parametros, pai)
{
    setProperty("papelVisual", QStringLiteral("subtitulo"));
    setWordWrap(true);
}
