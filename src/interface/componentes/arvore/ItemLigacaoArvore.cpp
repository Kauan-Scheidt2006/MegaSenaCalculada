#include "interface/componentes/arvore/ItemLigacaoArvore.hpp"

#include <QColor>
#include <QPen>

#include <utility>

ItemLigacaoArvore::ItemLigacaoArvore(
    apresentacao::arvore::ModeloLigacaoArvore ligacao,
    const QLineF& linha, const QColor& cor, qreal largura, QGraphicsItem* pai)
    : QGraphicsLineItem(linha, pai), ligacao_(std::move(ligacao)),
      corOriginal_(cor), larguraOriginal_(largura)
{
    QPen caneta(cor, largura);
    caneta.setCosmetic(true);
    setPen(caneta);
    setAcceptedMouseButtons(Qt::NoButton);
    setZValue(-1);
}

void ItemLigacaoArvore::definirDestaque(bool destacado, bool atenuado)
{
    QColor cor = corOriginal_;
    if (destacado) {
        cor = cor.darker(155);
    } else if (atenuado) {
        cor.setAlphaF(0.22);
    }
    QPen caneta(cor, destacado ? larguraOriginal_ * 2.0 : larguraOriginal_);
    caneta.setCosmetic(true);
    setPen(caneta);
}

const apresentacao::arvore::ModeloLigacaoArvore&
ItemLigacaoArvore::ligacao() const noexcept
{
    return ligacao_;
}
