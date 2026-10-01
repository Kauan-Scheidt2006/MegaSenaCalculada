#include "ArvoreVisualizacao.h"

#include <QCursor>
#include <QFrame>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSimpleTextItem>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QResizeEvent>
#include <QVBoxLayout>

#include <algorithm>
#include <functional>
#include <unordered_map>
#include <utility>
#include <vector>

namespace {

constexpr qreal DiametroNo = 72.0;
constexpr qreal EspacamentoHorizontal = 112.0;
constexpr qreal EspacamentoVertical = 120.0;

class ItemRegistro final : public QGraphicsEllipseItem {
public:
    ItemRegistro(const megasena::Registro& registro, const QPointF& posicao,
                 std::function<void(const megasena::Registro&, const QPointF&)> ativar)
        : QGraphicsEllipseItem(-DiametroNo / 2, -DiametroNo / 2,
                               DiametroNo, DiametroNo),
          registro_(registro), ativar_(std::move(ativar))
    {
        setPos(posicao);
        setFlag(ItemIsFocusable);
        setFlag(ItemIsSelectable);
        setToolTip(QStringLiteral("Abrir detalhes da dezena %1")
                       .arg(registro_.chave.numero));

        auto* texto = new QGraphicsSimpleTextItem(
            QString::number(registro_.chave.numero), this);
        QFont fonte = texto->font();
        fonte.setPointSize(18);
        fonte.setBold(true);
        texto->setFont(fonte);
        texto->setBrush(QColor("#17324d"));
        texto->setPos(-texto->boundingRect().width() / 2,
                      -texto->boundingRect().height() / 2);
        texto->setAcceptedMouseButtons(Qt::NoButton);
    }

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* evento) override
    {
        QGraphicsEllipseItem::mouseReleaseEvent(evento);
        if (evento->button() == Qt::LeftButton) {
            setFocus(Qt::MouseFocusReason);
            ativar_(registro_, scenePos());
        }
    }

    void keyPressEvent(QKeyEvent* evento) override
    {
        if (evento->key() == Qt::Key_Return || evento->key() == Qt::Key_Enter
            || evento->key() == Qt::Key_Space) {
            ativar_(registro_, scenePos());
            evento->accept();
            return;
        }
        QGraphicsEllipseItem::keyPressEvent(evento);
    }

    void paint(QPainter* pintor, const QStyleOptionGraphicsItem*, QWidget*) override
    {
        pintor->setRenderHint(QPainter::Antialiasing);
        const bool destacado = isSelected() || hasFocus();
        pintor->setBrush(destacado ? QColor("#dff7ec") : QColor("#e8f3fa"));
        pintor->setPen(QPen(destacado ? QColor("#087f5b") : QColor("#39728f"),
                            destacado ? 4 : 2));
        pintor->drawEllipse(rect());
    }

private:
    megasena::Registro registro_;
    std::function<void(const megasena::Registro&, const QPointF&)> ativar_;
};

struct NoVisual {
    const megasena::Registro* registro;
    const megasena::Registro* pai;
    std::size_t profundidade;
    QPointF posicao;
};

} // namespace

ArvoreVisualizacao::ArvoreVisualizacao(const ArvoreRegistros& arvore, QWidget* pai)
    : QGraphicsView(pai), cena_(new QGraphicsScene(this))
{
    setScene(cena_);
    setRenderHint(QPainter::Antialiasing);
    setDragMode(ScrollHandDrag);
    setTransformationAnchor(AnchorUnderMouse);
    setResizeAnchor(AnchorViewCenter);
    setBackgroundBrush(QColor("#f4f7fb"));
    setFrameShape(QFrame::NoFrame);
    setAccessibleName(QStringLiteral("Árvore binária de registros da Mega-Sena"));

    popover_ = new QFrame(viewport());
    popover_->setObjectName(QStringLiteral("popoverDetalhes"));
    popover_->setMinimumWidth(260);
    popover_->setFocusPolicy(Qt::StrongFocus);
    auto* disposicao = new QVBoxLayout(popover_);
    disposicao->setContentsMargins(18, 14, 18, 16);

    auto* cabecalho = new QHBoxLayout;
    numero_ = new QLabel;
    numero_->setObjectName(QStringLiteral("numeroDetalhe"));
    auto* fechar = new QPushButton(QStringLiteral("×"));
    fechar->setObjectName(QStringLiteral("fecharPopover"));
    fechar->setAccessibleName(QStringLiteral("Fechar detalhes"));
    fechar->setFixedSize(30, 30);
    cabecalho->addWidget(numero_);
    cabecalho->addStretch();
    cabecalho->addWidget(fechar);
    disposicao->addLayout(cabecalho);
    frequencia_ = new QLabel;
    concurso_ = new QLabel;
    disposicao->addWidget(frequencia_);
    disposicao->addWidget(concurso_);
    popover_->hide();

    connect(fechar, &QPushButton::clicked, this,
            [this] { fecharPopover(); });
    construirCena(arvore);
}

void ArvoreVisualizacao::construirCena(const ArvoreRegistros& arvore)
{
    std::vector<NoVisual> nos;
    std::size_t indiceHorizontal = 0;
    arvore.visitarNosEmOrdem(
        [&](const megasena::Registro& registro, const megasena::Registro* pai,
            bool, std::size_t profundidade) {
            nos.push_back({&registro, pai, profundidade,
                           {static_cast<qreal>(indiceHorizontal++) * EspacamentoHorizontal,
                            static_cast<qreal>(profundidade) * EspacamentoVertical}});
        });

    std::unordered_map<const megasena::Registro*, QPointF> posicoes;
    for (const NoVisual& no : nos) {
        posicoes.emplace(no.registro, no.posicao);
    }
    for (const NoVisual& no : nos) {
        if (no.pai) {
            auto* linha = cena_->addLine(QLineF(posicoes.at(no.pai), no.posicao),
                                         QPen(QColor("#91a4b7"), 2));
            linha->setZValue(-1);
        }
    }
    for (const NoVisual& no : nos) {
        cena_->addItem(new ItemRegistro(
            *no.registro, no.posicao,
            [this](const megasena::Registro& registro, const QPointF& posicao) {
                mostrarDetalhes(registro, posicao);
            }));
    }
    cena_->setSceneRect(cena_->itemsBoundingRect().adjusted(-70, -70, 70, 70));
}

void ArvoreVisualizacao::mostrarDetalhes(const megasena::Registro& registro,
                                         const QPointF& posicaoCena)
{
    ancoraPopover_ = posicaoCena;
    numero_->setText(QStringLiteral("Dezena %1").arg(registro.chave.numero));
    frequencia_->setText(QStringLiteral("Frequência acumulada: %1")
                             .arg(registro.chave.frequenciaAcumulada));
    concurso_->setText(QStringLiteral("Concurso: %1").arg(registro.numeroConcurso));
    popover_->setAccessibleName(
        QStringLiteral("Detalhes da dezena %1, frequência %2, concurso %3")
            .arg(registro.chave.numero)
            .arg(registro.chave.frequenciaAcumulada)
            .arg(registro.numeroConcurso));
    popover_->adjustSize();
    popover_->show();
    popover_->raise();
    reposicionarPopover();
    popover_->setFocus(Qt::OtherFocusReason);
}

void ArvoreVisualizacao::reposicionarPopover()
{
    if (!popover_->isVisible()) {
        return;
    }
    const QPoint ancora = mapFromScene(ancoraPopover_);
    int x = ancora.x() + static_cast<int>(DiametroNo / 2) + 12;
    int y = ancora.y() - popover_->height() / 2;
    if (x + popover_->width() > viewport()->width() - 10) {
        x = ancora.x() - popover_->width() - static_cast<int>(DiametroNo / 2) - 12;
    }
    x = std::clamp(x, 10, std::max(10, viewport()->width() - popover_->width() - 10));
    y = std::clamp(y, 10, std::max(10, viewport()->height() - popover_->height() - 10));
    popover_->move(x, y);
}

void ArvoreVisualizacao::fecharPopover()
{
    popover_->hide();
    setFocus(Qt::OtherFocusReason);
}

void ArvoreVisualizacao::enquadrarArvore()
{
    if (!cena_->items().isEmpty()) {
        fitInView(cena_->itemsBoundingRect().adjusted(-60, -60, 60, 60),
                  Qt::KeepAspectRatio);
    }
}

void ArvoreVisualizacao::resizeEvent(QResizeEvent* evento)
{
    QGraphicsView::resizeEvent(evento);
    reposicionarPopover();
}

void ArvoreVisualizacao::keyPressEvent(QKeyEvent* evento)
{
    if (evento->key() == Qt::Key_Escape && popover_->isVisible()) {
        fecharPopover();
        evento->accept();
        return;
    }
    QGraphicsView::keyPressEvent(evento);
}
