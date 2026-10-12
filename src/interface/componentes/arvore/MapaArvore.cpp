#include "interface/componentes/arvore/MapaArvore.hpp"

#include "interface/componentes/arvore/VisualizacaoArvore.hpp"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QResizeEvent>

#include <algorithm>

MapaArvore::MapaArvore(QWidget* pai)
    : QGraphicsView(pai)
{
    setObjectName(QStringLiteral("mapaArvore"));
    setAccessibleName(QStringLiteral("Minimapa da arvore"));
    setAccessibleDescription(
        QStringLiteral("Mostra a area visivel da arvore. Clique, arraste ou use as setas para navegar."));
    setToolTip(QStringLiteral("Clique ou arraste para navegar pela arvore"));
    setFocusPolicy(Qt::StrongFocus);
    setCursor(Qt::OpenHandCursor);
    setInteractive(false);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void MapaArvore::vincular(VisualizacaoArvore& visualizacao)
{
    visualizacao_ = &visualizacao;
    setScene(visualizacao.scene());
    connect(&visualizacao, &VisualizacaoArvore::viewportAlterado,
            this, qOverload<>(&MapaArvore::update));
    sincronizar();
}

void MapaArvore::sincronizar()
{
    if (scene() && !scene()->sceneRect().isEmpty()) {
        fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
    }
    viewport()->update();
}

QColor MapaArvore::corAreaVisivel() const noexcept { return corAreaVisivel_; }
void MapaArvore::definirCorAreaVisivel(const QColor& cor) noexcept { corAreaVisivel_ = cor; }
QColor MapaArvore::corBordaAreaVisivel() const noexcept { return corBordaAreaVisivel_; }
void MapaArvore::definirCorBordaAreaVisivel(const QColor& cor) noexcept { corBordaAreaVisivel_ = cor; }
qreal MapaArvore::larguraBordaAreaVisivel() const noexcept { return larguraBordaAreaVisivel_; }
void MapaArvore::definirLarguraBordaAreaVisivel(qreal largura) noexcept { larguraBordaAreaVisivel_ = largura; }

void MapaArvore::drawForeground(QPainter* pintor, const QRectF& area)
{
    QGraphicsView::drawForeground(pintor, area);
    if (!visualizacao_) return;
    QPen caneta(corBordaAreaVisivel_, larguraBordaAreaVisivel_);
    caneta.setCosmetic(true);
    pintor->setPen(caneta);
    pintor->setBrush(corAreaVisivel_);
    pintor->drawRect(areaVisivelLimitada());
}

void MapaArvore::mousePressEvent(QMouseEvent* evento)
{
    if (visualizacao_ && evento->button() == Qt::LeftButton) {
        arrastandoViewport_ = true;
        setCursor(Qt::ClosedHandCursor);
        centralizarVisualizacao(evento->position().toPoint());
        evento->accept();
        return;
    }
    QGraphicsView::mousePressEvent(evento);
}

void MapaArvore::mouseMoveEvent(QMouseEvent* evento)
{
    if (arrastandoViewport_ && (evento->buttons() & Qt::LeftButton)) {
        centralizarVisualizacao(evento->position().toPoint());
        evento->accept();
        return;
    }
    QGraphicsView::mouseMoveEvent(evento);
}

void MapaArvore::mouseReleaseEvent(QMouseEvent* evento)
{
    if (arrastandoViewport_ && evento->button() == Qt::LeftButton) {
        centralizarVisualizacao(evento->position().toPoint());
        arrastandoViewport_ = false;
        setCursor(Qt::OpenHandCursor);
        evento->accept();
        return;
    }
    QGraphicsView::mouseReleaseEvent(evento);
}

void MapaArvore::keyPressEvent(QKeyEvent* evento)
{
    switch (evento->key()) {
    case Qt::Key_Left: deslocarViewport(-0.25, 0); break;
    case Qt::Key_Right: deslocarViewport(0.25, 0); break;
    case Qt::Key_Up: deslocarViewport(0, -0.25); break;
    case Qt::Key_Down: deslocarViewport(0, 0.25); break;
    default:
        QGraphicsView::keyPressEvent(evento);
        return;
    }
    evento->accept();
}

void MapaArvore::resizeEvent(QResizeEvent* evento)
{
    QGraphicsView::resizeEvent(evento);
    sincronizar();
}

QRectF MapaArvore::areaVisivelLimitada() const
{
    if (!visualizacao_ || !scene()) return {};
    const QRectF areaVisivel = visualizacao_->mapToScene(
        visualizacao_->viewport()->rect()).boundingRect();
    return areaVisivel.intersected(scene()->sceneRect());
}

void MapaArvore::centralizarVisualizacao(const QPoint& posicaoMapa)
{
    if (!visualizacao_ || !scene()) return;
    const QPointF posicaoCena = mapToScene(posicaoMapa);
    const QRectF limites = scene()->sceneRect();
    visualizacao_->centralizarEm({
        std::clamp(posicaoCena.x(), limites.left(), limites.right()),
        std::clamp(posicaoCena.y(), limites.top(), limites.bottom())});
}

void MapaArvore::deslocarViewport(qreal horizontal, qreal vertical)
{
    if (!visualizacao_) return;
    const QRectF areaVisivel = visualizacao_->mapToScene(
        visualizacao_->viewport()->rect()).boundingRect();
    visualizacao_->centralizarEm(
        areaVisivel.center()
        + QPointF(areaVisivel.width() * horizontal,
                  areaVisivel.height() * vertical));
}
