#include "interface/componentes/basicos/BotaoIcone.hpp"

#include <QIcon>

BotaoIcone::BotaoIcone(const QString& recursoIcone, const QString& textoAlternativo,
                       const ParametrosComponente& parametros, QWidget* pai)
    : Botao({}, parametros, pai)
{
    setAccessibleName(textoAlternativo);
    definirIcone(recursoIcone);
}

void BotaoIcone::definirIcone(const QString& recursoIcone)
{
    setIcon(QIcon(recursoIcone));
}
