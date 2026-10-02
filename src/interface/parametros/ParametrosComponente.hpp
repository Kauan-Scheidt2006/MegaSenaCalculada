#pragma once

#include <QSize>
#include <QSizePolicy>
#include <QString>

#include <optional>

/** Configuração visual comum que não altera a propriedade Qt do componente. */
struct ParametrosComponente
{
    QString nomeObjeto;
    QString nomeAcessivel;
    QString descricaoAcessivel;
    std::optional<QSize> tamanhoMinimo;
    std::optional<QSize> tamanhoMaximo;
    std::optional<QSizePolicy> politicaTamanho;
    std::optional<bool> fundoEstilizado;
    bool visivel = true;
};
