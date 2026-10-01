#include "interface/componentes/basicos/Botao.hpp"

#include "interface/fundamentos/ConfiguradorComponente.hpp"

Botao::Botao(const QString& texto, const ParametrosComponente& parametros, QWidget* pai)
    : QPushButton(texto, pai)
{
    ConfiguradorComponente::aplicar(*this, parametros);
    setFocusPolicy(Qt::StrongFocus);
    if (accessibleName().isEmpty()) {
        setAccessibleName(texto);
    }
}

void Botao::definirTexto(const QString& texto)
{
    const QString textoAnterior = this->text();
    const bool nomeAcessivelAutomatico = accessibleName().isEmpty() || accessibleName() == textoAnterior;
    setText(texto);
    if (nomeAcessivelAutomatico) {
        setAccessibleName(texto);
    }
}
