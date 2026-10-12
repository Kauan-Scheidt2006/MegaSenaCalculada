#pragma once

#include "interface/apresentacao/arvore/ModeloNoArvore.hpp"

#include <QColor>
#include <QGraphicsObject>

#include <functional>

struct EstiloItemNoArvore {
    struct Paleta {
        QColor fundo;
        QColor borda;
        QColor texto;
        QColor fundoSelecionado;
        QColor bordaSelecionada;
        QColor textoSelecionado;
    };

    qreal diametro;
    qreal larguraBorda;
    qreal larguraBordaFoco;
    Paleta padrao;
    Paleta vermelho;
    qreal tamanhoFonteNumero;
    qreal tamanhoFonteFrequencia;
};

class ItemNoArvore final : public QGraphicsObject
{
public:
    using AcaoAtivacao = std::function<void(
        apresentacao::arvore::IdentificadorRegistro)>;

    ItemNoArvore(apresentacao::arvore::IdentificadorRegistro identificador,
                 apresentacao::arvore::SemanticaVisualNo semanticaVisual,
                 EstiloItemNoArvore estilo, AcaoAtivacao ativar,
                 QGraphicsItem* pai = nullptr);

    [[nodiscard]] const apresentacao::arvore::IdentificadorRegistro&
    identificador() const noexcept;
    [[nodiscard]] apresentacao::arvore::SemanticaVisualNo
    semanticaVisual() const noexcept;
    [[nodiscard]] QRectF boundingRect() const override;
    [[nodiscard]] QPainterPath shape() const override;
    void definirAtenuado(bool atenuado);
    void paint(QPainter* pintor, const QStyleOptionGraphicsItem* opcao,
               QWidget* widget) override;

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* evento) override;
    void keyReleaseEvent(QKeyEvent* evento) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* evento) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* evento) override;

private:
    void ativar();

    apresentacao::arvore::IdentificadorRegistro identificador_;
    apresentacao::arvore::SemanticaVisualNo semanticaVisual_;
    EstiloItemNoArvore estilo_;
    AcaoAtivacao acaoAtivacao_;
    bool sobMouse_ = false;
    bool atenuado_ = false;
};
