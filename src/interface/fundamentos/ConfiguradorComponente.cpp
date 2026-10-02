#include "interface/fundamentos/ConfiguradorComponente.hpp"

#include "interface/parametros/ParametrosComponente.hpp"

#include <QWidget>

#include <stdexcept>

void ConfiguradorComponente::aplicar(QWidget& componente, const ParametrosComponente& parametros)
{
    if (parametros.nomeObjeto.isEmpty()) {
        throw std::invalid_argument("O nomeObjeto do componente deve ser definido.");
    }
    if (!parametros.fundoEstilizado.has_value()) {
        throw std::invalid_argument("O fundoEstilizado do componente deve ser definido.");
    }

    componente.setObjectName(parametros.nomeObjeto);
    componente.setAttribute(Qt::WA_StyledBackground, *parametros.fundoEstilizado);
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
