#pragma once

#include "dominio/Registro.hpp"
#include "estruturas/ArvoreBinariaBusca.hpp"

#include <QGraphicsView>

class QFrame;
class QGraphicsScene;
class QLabel;

class ArvoreVisualizacao final : public QGraphicsView {
public:
    using ArvoreRegistros = estruturas::ArvoreBinariaBusca<
        megasena::Registro, megasena::ComparadorRegistro>;

    explicit ArvoreVisualizacao(const ArvoreRegistros& arvore,
                                QWidget* pai = nullptr);

    void enquadrarArvore();

protected:
    void resizeEvent(QResizeEvent* evento) override;
    void keyPressEvent(QKeyEvent* evento) override;

private:
    void construirCena(const ArvoreRegistros& arvore);
    void mostrarDetalhes(const megasena::Registro& registro,
                         const QPointF& posicaoCena);
    void reposicionarPopover();
    void fecharPopover();

    QGraphicsScene* cena_ = nullptr;
    QFrame* popover_ = nullptr;
    QLabel* numero_ = nullptr;
    QLabel* frequencia_ = nullptr;
    QLabel* concurso_ = nullptr;
    QPointF ancoraPopover_;
};
