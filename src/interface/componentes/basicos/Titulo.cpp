#include "interface/componentes/basicos/Titulo.hpp"

Titulo::Titulo(const QString& conteudo, ParametrosComponente parametros, QWidget* pai)
    : Texto(conteudo, parametros, pai)
{
    setProperty("papelVisual", QStringLiteral("titulo"));
}
