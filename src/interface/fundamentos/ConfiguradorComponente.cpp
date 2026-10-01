#include "interface/fundamentos/ConfiguradorComponente.hpp"

#include "interface/parametros/ParametrosComponente.hpp"

#include <QWidget>

void ConfiguradorComponente::aplicar(QWidget& componente, const ParametrosComponente& parametros)
{
    componente.setObjectName(parametros.nome);
    componente.setAttribute(Qt::WA_StyledBackground, parametros.fundoEstilizado);
    componente.setVisible(parametros.visivel);

    if (!parametros.nomeAcessivel.isEmpty()) {
        componente.setAccessibleName(parametros.nomeAcessivel);
    }
    if (!parametros.descricaoAcessivel.isEmpty()) {
        componente.setAccessibleDescription(parametros.descricaoAcessivel);
    }
    if (parametros.tamanhoMinimo) {
        componente.setMinimumSize(*parametros.tamanhoMinimo);
    }
    if (parametros.tamanhoMaximo) {
        componente.setMaximumSize(*parametros.tamanhoMaximo);
    }
    if (parametros.politicaTamanho) {
        componente.setSizePolicy(*parametros.politicaTamanho);
    }
}
