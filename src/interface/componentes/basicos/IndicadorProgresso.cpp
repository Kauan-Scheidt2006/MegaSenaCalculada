#include "interface/componentes/basicos/IndicadorProgresso.hpp"

#include "interface/fundamentos/ConfiguradorComponente.hpp"

#include <stdexcept>

IndicadorProgresso::IndicadorProgresso(const ParametrosComponente& parametros, QWidget* pai)
    : QProgressBar(pai)
{
    ConfiguradorComponente::aplicar(*this, parametros);
    setTextVisible(true);
}

void IndicadorProgresso::definirProgresso(int atual, int total)
{
    if (total <= 0 || atual < 0 || atual > total) {
        throw std::invalid_argument("O progresso deve estar entre zero e o total positivo.");
    }
    setRange(0, total);
    setValue(atual);
    setAccessibleDescription(QStringLiteral("Progresso: %1 de %2").arg(atual).arg(total));
}

void IndicadorProgresso::definirIndeterminado()
{
    setRange(0, 0);
    setAccessibleDescription(QStringLiteral("Operacao em andamento"));
}
