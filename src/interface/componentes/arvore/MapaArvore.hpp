#pragma once

#include <QColor>
#include <QGraphicsView>

class VisualizacaoArvore;

class MapaArvore final : public QGraphicsView
{
    Q_OBJECT
    Q_PROPERTY(QColor corAreaVisivel READ corAreaVisivel WRITE definirCorAreaVisivel)
    Q_PROPERTY(QColor corBordaAreaVisivel READ corBordaAreaVisivel WRITE definirCorBordaAreaVisivel)
    Q_PROPERTY(qreal larguraBordaAreaVisivel READ larguraBordaAreaVisivel WRITE definirLarguraBordaAreaVisivel)

public:
    explicit MapaArvore(QWidget* pai = nullptr);

    void vincular(VisualizacaoArvore& visualizacao);
    void sincronizar();

    [[nodiscard]] QColor corAreaVisivel() const noexcept;
    void definirCorAreaVisivel(const QColor& cor) noexcept;
    [[nodiscard]] QColor corBordaAreaVisivel() const noexcept;
    void definirCorBordaAreaVisivel(const QColor& cor) noexcept;
    [[nodiscard]] qreal larguraBordaAreaVisivel() const noexcept;
    void definirLarguraBordaAreaVisivel(qreal largura) noexcept;

protected:
    void drawForeground(QPainter* pintor, const QRectF& area) override;
    void mousePressEvent(QMouseEvent* evento) override;
    void mouseMoveEvent(QMouseEvent* evento) override;
    void mouseReleaseEvent(QMouseEvent* evento) override;
    void keyPressEvent(QKeyEvent* evento) override;
    void resizeEvent(QResizeEvent* evento) override;

private:
    [[nodiscard]] QRectF areaVisivelLimitada() const;
    void centralizarVisualizacao(const QPoint& posicaoMapa);
    void deslocarViewport(qreal horizontal, qreal vertical);

    VisualizacaoArvore* visualizacao_ = nullptr;
    QColor corAreaVisivel_;
    QColor corBordaAreaVisivel_;
    qreal larguraBordaAreaVisivel_ = 0;
    bool arrastandoViewport_ = false;
};
