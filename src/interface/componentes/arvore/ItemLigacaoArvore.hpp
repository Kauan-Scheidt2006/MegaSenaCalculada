#pragma once

#include "interface/apresentacao/arvore/ModeloLigacaoArvore.hpp"

#include <QColor>
#include <QGraphicsLineItem>

class ItemLigacaoArvore final : public QGraphicsLineItem
{
public:
    ItemLigacaoArvore(
        apresentacao::arvore::ModeloLigacaoArvore ligacao,
        const QLineF& linha, const QColor& cor, qreal largura,
        QGraphicsItem* pai = nullptr);

    [[nodiscard]] const apresentacao::arvore::ModeloLigacaoArvore&
    ligacao() const noexcept;
    void definirDestaque(bool destacado, bool atenuado = false);

private:
    apresentacao::arvore::ModeloLigacaoArvore ligacao_;
    QColor corOriginal_;
    qreal larguraOriginal_ = 0;
};
