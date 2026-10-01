#include "interface/componentes/basicos/Imagem.hpp"

#include "interface/fundamentos/ConfiguradorComponente.hpp"

#include <QPixmap>

Imagem::Imagem(const QString& recurso, const QString& textoAlternativo,
               const ParametrosComponente& parametros, QWidget* pai)
    : QLabel(pai)
{
    ConfiguradorComponente::aplicar(*this, parametros);
    setScaledContents(true);
    definirTextoAlternativo(textoAlternativo);
    definirRecurso(recurso);
}

void Imagem::definirRecurso(const QString& recurso)
{
    setPixmap(QPixmap(recurso));
}

void Imagem::definirTextoAlternativo(const QString& textoAlternativo)
{
    setAccessibleName(textoAlternativo);
}
