#include "interface/componentes/arvore/ItemNoArvore.hpp"

#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionGraphicsItem>

#include <algorithm>
#include <utility>

ItemNoArvore::ItemNoArvore(
    apresentacao::arvore::IdentificadorRegistro identificador,
    apresentacao::arvore::SemanticaVisualNo semanticaVisual,
    EstiloItemNoArvore estilo, AcaoAtivacao ativar, QGraphicsItem* pai)
    : QGraphicsObject(pai), identificador_(identificador),
      semanticaVisual_(semanticaVisual),
      estilo_(std::move(estilo)), acaoAtivacao_(std::move(ativar))
{
    setFlag(ItemIsFocusable);
    setFlag(ItemIsSelectable);
    setAcceptHoverEvents(true);
    QString descricaoCor;
    if (semanticaVisual_ == apresentacao::arvore::SemanticaVisualNo::RubroNegroPreto) {
        descricaoCor = QStringLiteral(" Nó rubro-negro preto.");
    } else if (semanticaVisual_ == apresentacao::arvore::SemanticaVisualNo::RubroNegroVermelho) {
        descricaoCor = QStringLiteral(" Nó rubro-negro vermelho.");
    }
    setToolTip(QStringLiteral("Dezena %1, frequencia acumulada %2.%3 Pressione Enter ou Espaco para abrir os detalhes.")
                   .arg(identificador_.numero)
                   .arg(identificador_.frequenciaAcumulada)
                   .arg(descricaoCor));
}

apresentacao::arvore::SemanticaVisualNo
ItemNoArvore::semanticaVisual() const noexcept
{
    return semanticaVisual_;
}

const apresentacao::arvore::IdentificadorRegistro&
ItemNoArvore::identificador() const noexcept
{
    return identificador_;
}

QRectF ItemNoArvore::boundingRect() const
{
    const qreal metade = estilo_.diametro / 2.0;
    const qreal margem = std::max(estilo_.larguraBorda,
                                  estilo_.larguraBordaFoco) / 2.0;
    return {-metade - margem, -metade - margem,
            estilo_.diametro + 2 * margem, estilo_.diametro + 2 * margem};
}

QPainterPath ItemNoArvore::shape() const
{
    const qreal metade = estilo_.diametro / 2.0;
    QPainterPath caminho;
    caminho.addEllipse(QRectF(-metade, -metade,
                              estilo_.diametro, estilo_.diametro));
    return caminho;
}

void ItemNoArvore::definirAtenuado(bool atenuado)
{
    atenuado_ = atenuado;
    setOpacity(atenuado ? 0.28 : 1.0);
    update();
}

void ItemNoArvore::paint(QPainter* pintor,
                         const QStyleOptionGraphicsItem* opcao, QWidget*)
{
    const bool destacado = isSelected() || hasFocus() || sobMouse_;
    const auto& paleta =
        semanticaVisual_ == apresentacao::arvore::SemanticaVisualNo::RubroNegroVermelho
        ? estilo_.vermelho
        : estilo_.padrao;
    const qreal nivelDetalhe = QStyleOptionGraphicsItem::levelOfDetailFromTransform(
        pintor->worldTransform());
    if (nivelDetalhe < 0.08) {
        QPen ponto(destacado ? paleta.bordaSelecionada : paleta.borda, 1);
        ponto.setCosmetic(true);
        pintor->setPen(ponto);
        pintor->drawPoint(QPointF{});
        return;
    }
    const qreal metade = estilo_.diametro / 2.0;
    const QRectF circulo(-metade, -metade,
                         estilo_.diametro, estilo_.diametro);
    pintor->setRenderHint(QPainter::Antialiasing);
    pintor->setBrush(destacado ? paleta.fundoSelecionado : paleta.fundo);
    pintor->setPen(QPen(destacado ? paleta.bordaSelecionada : paleta.borda,
                        hasFocus() ? estilo_.larguraBordaFoco
                                   : estilo_.larguraBorda));
    pintor->drawEllipse(circulo);
    pintor->setPen(destacado ? paleta.textoSelecionado : paleta.texto);
    if (nivelDetalhe >= 0.18 || opcao->state.testFlag(QStyle::State_Selected)) {
        QFont fonteNumero = pintor->font();
        fonteNumero.setPointSizeF(estilo_.tamanhoFonteNumero);
        fonteNumero.setBold(true);
        pintor->setFont(fonteNumero);
        const QRectF areaNumero(circulo.left(), circulo.top() + circulo.height() * 0.12,
                                circulo.width(), circulo.height() * 0.48);
        pintor->drawText(areaNumero, Qt::AlignCenter,
                         QStringLiteral("%1").arg(identificador_.numero, 2, 10, QLatin1Char('0')));

        if (nivelDetalhe >= 0.45 || opcao->state.testFlag(QStyle::State_Selected)) {
            QFont fonteFrequencia = fonteNumero;
            fonteFrequencia.setPointSizeF(estilo_.tamanhoFonteFrequencia);
            fonteFrequencia.setBold(false);
            pintor->setFont(fonteFrequencia);
            const QRectF areaFrequencia(circulo.left(), circulo.top() + circulo.height() * 0.51,
                                        circulo.width(), circulo.height() * 0.30);
            pintor->drawText(areaFrequencia, Qt::AlignCenter,
                             QStringLiteral("%1x").arg(identificador_.frequenciaAcumulada));
        }
    }
}

void ItemNoArvore::mouseReleaseEvent(QGraphicsSceneMouseEvent* evento)
{
    QGraphicsObject::mouseReleaseEvent(evento);
    if (evento->button() == Qt::LeftButton
        && shape().contains(evento->pos())) {
        ativar();
        evento->accept();
    }
}

void ItemNoArvore::keyReleaseEvent(QKeyEvent* evento)
{
    if (evento->key() == Qt::Key_Return || evento->key() == Qt::Key_Enter
        || evento->key() == Qt::Key_Space) {
        ativar();
        evento->accept();
        return;
    }
    QGraphicsObject::keyReleaseEvent(evento);
}

void ItemNoArvore::hoverEnterEvent(QGraphicsSceneHoverEvent* evento)
{
    sobMouse_ = true;
    update();
    QGraphicsObject::hoverEnterEvent(evento);
}

void ItemNoArvore::hoverLeaveEvent(QGraphicsSceneHoverEvent* evento)
{
    sobMouse_ = false;
    update();
    QGraphicsObject::hoverLeaveEvent(evento);
}

void ItemNoArvore::ativar()
{
    if (scene()) {
        scene()->clearSelection();
    }
    setSelected(true);
    setFocus(Qt::OtherFocusReason);
    if (acaoAtivacao_) {
        acaoAtivacao_(identificador_);
    }
}
