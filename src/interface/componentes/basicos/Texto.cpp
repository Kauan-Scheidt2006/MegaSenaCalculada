#include "interface/componentes/basicos/Texto.hpp"

#include "interface/fundamentos/ConfiguradorComponente.hpp"

Texto::Texto(const QString& conteudo, const ParametrosComponente& parametros, QWidget* pai)
    : QLabel(conteudo, pai)
{
    ConfiguradorComponente::aplicar(*this, parametros);
    if (accessibleName().isEmpty()) {
        setAccessibleName(conteudo);
    }
}

void Texto::definirConteudo(const QString& conteudo)
{
    const QString conteudoAnterior = text();
    const bool nomeAcessivelAutomatico = accessibleName().isEmpty() || accessibleName() == conteudoAnterior;
    setText(conteudo);
    if (nomeAcessivelAutomatico) {
        setAccessibleName(conteudo);
    }
}
