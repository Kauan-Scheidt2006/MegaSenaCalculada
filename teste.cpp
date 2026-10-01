#include <QApplication>
#include <QCursor>
#include <QEasingCurve>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsEllipseItem>
#include <QGraphicsOpacityEffect>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QMainWindow>
#include <QPainter>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QScrollBar>
#include <QShortcut>
#include <QTimer>
#include <QVBoxLayout>
#include <QWheelEvent>

#include <algorithm>
#include <functional>
#include <memory>
#include <optional>
#include <vector>

namespace {

constexpr qreal DiametroNo = 70.0;
constexpr qreal EspacamentoHorizontal = 112.0;
constexpr qreal EspacamentoVertical = 125.0;
constexpr int DuracaoAnimacao = 190;

enum class TipoApresentacao {
    ModalCentral = 1,
    GavetaModal = 2,
    Popover = 3,
    InspetorLateral = 4,
    PainelInferior = 5
};

QString nomeApresentacao(TipoApresentacao tipo)
{
    switch (tipo) {
    case TipoApresentacao::ModalCentral: return QStringLiteral("Modal central");
    case TipoApresentacao::GavetaModal: return QStringLiteral("Drawer modal");
    case TipoApresentacao::Popover: return QStringLiteral("Popover");
    case TipoApresentacao::InspetorLateral: return QStringLiteral("Inspetor lateral");
    case TipoApresentacao::PainelInferior: return QStringLiteral("Painel inferior");
    }
    return {};
}

struct NoArvore {
    explicit NoArvore(int novoValor) : valor(novoValor) {}
    int valor;
    std::unique_ptr<NoArvore> esquerdo;
    std::unique_ptr<NoArvore> direito;
    QPointF posicao;
};

void inserirNo(std::unique_ptr<NoArvore>& no, int valor)
{
    if (!no) no = std::make_unique<NoArvore>(valor);
    else if (valor < no->valor) inserirNo(no->esquerdo, valor);
    else inserirNo(no->direito, valor);
}

void calcularPosicoes(NoArvore* no, int profundidade, int& indiceHorizontal)
{
    if (!no) return;
    calcularPosicoes(no->esquerdo.get(), profundidade + 1, indiceHorizontal);
    no->posicao = {indiceHorizontal++ * EspacamentoHorizontal,
                   profundidade * EspacamentoVertical};
    calcularPosicoes(no->direito.get(), profundidade + 1, indiceHorizontal);
}

using AcaoAoAtivar =
    std::function<void(int, TipoApresentacao, const QPointF&, const QPoint&)>;

class ItemNo final : public QGraphicsEllipseItem {
public:
    ItemNo(int valor, TipoApresentacao tipo, const QPointF& centro,
           AcaoAoAtivar acao)
        : QGraphicsEllipseItem(-DiametroNo / 2, -DiametroNo / 2,
                               DiametroNo, DiametroNo),
          valor_(valor), tipo_(tipo), acao_(std::move(acao))
    {
        setPos(centro);
        setFlag(ItemIsSelectable);
        setFlag(ItemIsFocusable);
        setToolTip(nomeApresentacao(tipo_));

        auto* valorVisual = new QGraphicsSimpleTextItem(QString::number(valor_), this);
        QFont fonte = valorVisual->font();
        fonte.setBold(true);
        fonte.setPointSize(11);
        valorVisual->setFont(fonte);
        valorVisual->setBrush(QColor("#17324d"));
        valorVisual->setPos(-valorVisual->boundingRect().width() / 2, -22);
        valorVisual->setAcceptedMouseButtons(Qt::NoButton);

        auto* tipoVisual = new QGraphicsSimpleTextItem(nomeApresentacao(tipo_), this);
        fonte.setBold(false);
        fonte.setPointSize(6);
        tipoVisual->setFont(fonte);
        tipoVisual->setBrush(QColor("#50657a"));
        tipoVisual->setPos(-tipoVisual->boundingRect().width() / 2, 8);
        tipoVisual->setAcceptedMouseButtons(Qt::NoButton);
    }

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* evento) override
    {
        QGraphicsEllipseItem::mouseReleaseEvent(evento);
        if (evento->button() == Qt::LeftButton && acao_) {
            setFocus(Qt::MouseFocusReason);
            acao_(valor_, tipo_, scenePos(), evento->screenPos());
        }
    }

    void keyPressEvent(QKeyEvent* evento) override
    {
        if ((evento->key() == Qt::Key_Return || evento->key() == Qt::Key_Enter
             || evento->key() == Qt::Key_Space) && acao_) {
            acao_(valor_, tipo_, scenePos(), QCursor::pos());
            evento->accept();
            return;
        }
        QGraphicsEllipseItem::keyPressEvent(evento);
    }

    void paint(QPainter* pintor, const QStyleOptionGraphicsItem*, QWidget*) override
    {
        const bool destacado = isSelected() || hasFocus();
        pintor->setRenderHint(QPainter::Antialiasing);
        pintor->setBrush(destacado ? QColor("#61d19a") : QColor("#e8f3fa"));
        pintor->setPen(QPen(destacado ? QColor("#087f5b") : QColor("#39728f"),
                            destacado ? 4 : 2));
        pintor->drawEllipse(rect());
        if (hasFocus()) {
            pintor->setPen(QPen(QColor("#ffb020"), 2, Qt::DashLine));
            pintor->drawEllipse(rect().adjusted(-5, -5, 5, 5));
        }
    }

private:
    int valor_;
    TipoApresentacao tipo_;
    AcaoAoAtivar acao_;
};

class VisualizacaoArvore final : public QGraphicsView {
public:
    explicit VisualizacaoArvore(QGraphicsScene* cena, QWidget* pai = nullptr)
        : QGraphicsView(cena, pai)
    {
        setRenderHint(QPainter::Antialiasing);
        setDragMode(ScrollHandDrag);
        setTransformationAnchor(AnchorUnderMouse);
        setResizeAnchor(AnchorViewCenter);
        setBackgroundBrush(QColor("#f4f7fb"));
        setFrameShape(QFrame::NoFrame);
    }

    void definirAoEscapar(std::function<void()> acao) { aoEscapar_ = std::move(acao); }
    void definirAoTransformar(std::function<void()> acao)
    {
        aoTransformar_ = std::move(acao);
    }
    void enquadrarTudo()
    {
        if (scene() && !scene()->items().isEmpty()) {
            fitInView(scene()->itemsBoundingRect().adjusted(-70, -70, 70, 70),
                      Qt::KeepAspectRatio);
            if (aoTransformar_) aoTransformar_();
        }
    }
    void aproximar() { aplicarZoom(1.20); }
    void afastar() { aplicarZoom(1.0 / 1.20); }

protected:
    void wheelEvent(QWheelEvent* evento) override
    {
        aplicarZoom(evento->angleDelta().y() > 0 ? 1.15 : 1.0 / 1.15);
        evento->accept();
    }
    void keyPressEvent(QKeyEvent* evento) override
    {
        if (evento->key() == Qt::Key_Escape && aoEscapar_) aoEscapar_();
        else if (evento->key() == Qt::Key_Plus || evento->key() == Qt::Key_Equal)
            aproximar();
        else if (evento->key() == Qt::Key_Minus) afastar();
        else if (evento->key() == Qt::Key_0) enquadrarTudo();
        else QGraphicsView::keyPressEvent(evento);
    }

private:
    void aplicarZoom(qreal fator)
    {
        const qreal escala = transform().m11() * fator;
        if (escala >= 0.08 && escala <= 8.0) {
            scale(fator, fator);
            if (aoTransformar_) aoTransformar_();
        }
    }
    std::function<void()> aoEscapar_;
    std::function<void()> aoTransformar_;
};

QFrame* criarCartao(QWidget* pai, QLabel*& titulo, QPushButton*& botaoFechar)
{
    auto* cartao = new QFrame(pai);
    cartao->setObjectName(QStringLiteral("cartaoDetalhes"));
    cartao->setMinimumWidth(300);
    auto* sombra = new QGraphicsDropShadowEffect(cartao);
    sombra->setBlurRadius(28);
    sombra->setOffset(0, 8);
    sombra->setColor(QColor(15, 35, 55, 80));
    cartao->setGraphicsEffect(sombra);

    auto* disposicao = new QVBoxLayout(cartao);
    disposicao->setContentsMargins(22, 18, 22, 20);
    disposicao->setSpacing(14);
    auto* cabecalho = new QHBoxLayout;
    auto* secao = new QLabel(QStringLiteral("DETALHES DO NÓ"));
    secao->setObjectName(QStringLiteral("rotuloSecao"));
    botaoFechar = new QPushButton(QStringLiteral("×"));
    botaoFechar->setObjectName(QStringLiteral("botaoFechar"));
    botaoFechar->setFixedSize(32, 32);
    botaoFechar->setAccessibleName(QStringLiteral("Fechar detalhes"));
    cabecalho->addWidget(secao);
    cabecalho->addStretch();
    cabecalho->addWidget(botaoFechar);
    disposicao->addLayout(cabecalho);

    titulo = new QLabel;
    titulo->setObjectName(QStringLiteral("tituloConcurso"));
    disposicao->addWidget(titulo);
    auto* descricao = new QLabel(QStringLiteral("Dezenas apresentadas"));
    descricao->setObjectName(QStringLiteral("rotuloSecao"));
    disposicao->addWidget(descricao);
    auto* dezenas = new QHBoxLayout;
    dezenas->setSpacing(8);
    for (const QString& numero : {"1", "10", "20", "30", "40", "50"}) {
        auto* marcador = new QLabel(numero);
        marcador->setObjectName(QStringLiteral("marcadorDezena"));
        marcador->setAlignment(Qt::AlignCenter);
        marcador->setFixedSize(38, 38);
        dezenas->addWidget(marcador);
    }
    dezenas->addStretch();
    disposicao->addLayout(dezenas);
    return cartao;
}

class ApresentadorDetalhes final : public QObject {
public:
    ApresentadorDetalhes(VisualizacaoArvore* visualizacao, QObject* pai = nullptr)
        : QObject(pai), visualizacao_(visualizacao), superficie_(visualizacao->viewport())
    {
        superficie_->installEventFilter(this);
        visualizacao_->horizontalScrollBar()->installEventFilter(this);
        visualizacao_->verticalScrollBar()->installEventFilter(this);
        visualizacao_->definirAoEscapar([this] { fecharTudo(); });
        visualizacao_->definirAoTransformar([this] { reposicionarPopover(); });
        auto* atalhoFechar = new QShortcut(QKeySequence(Qt::Key_Escape), superficie_);
        atalhoFechar->setContext(Qt::WidgetWithChildrenShortcut);
        QObject::connect(atalhoFechar, &QShortcut::activated,
                         this, [this] { fecharTudo(); });
    }

    void apresentar(int valor, TipoApresentacao tipo, const QPointF& posicaoCena)
    {
        QGraphicsItem* novoNoOrigem = visualizacao_->scene()->focusItem();
        fecharTudo();
        noOrigem_ = novoNoOrigem;
        concursoAtual_ = 1000 + valor;
        posicaoPopover_ = posicaoCena;
        tipoAtual_ = tipo;
        switch (tipo) {
        case TipoApresentacao::ModalCentral: abrirModalCentral(); break;
        case TipoApresentacao::GavetaModal: abrirGavetaModal(); break;
        case TipoApresentacao::Popover: abrirPopover(); break;
        case TipoApresentacao::InspetorLateral: abrirInspetor(); break;
        case TipoApresentacao::PainelInferior: abrirPainelInferior(); break;
        }
    }

    void fecharTudo()
    {
        for (QWidget* componente : {camadaModal_, popover_, inspetor_, painelInferior_})
            if (componente) componente->hide();
        tipoAtual_.reset();
        visualizacao_->setFocus(Qt::OtherFocusReason);
        if (noOrigem_) noOrigem_->setFocus(Qt::OtherFocusReason);
    }

protected:
    bool eventFilter(QObject* observado, QEvent* evento) override
    {
        if (observado == superficie_ && evento->type() == QEvent::Resize) {
            ajustarComponentes();
        }
        if ((observado == visualizacao_->horizontalScrollBar()
             || observado == visualizacao_->verticalScrollBar())
            && evento->type() == QEvent::Paint) {
            reposicionarPopover();
        }
        return QObject::eventFilter(observado, evento);
    }

private:
    void atualizarTitulo(QLabel* titulo) const
    {
        titulo->setText(QStringLiteral("Concurso %1").arg(concursoAtual_));
        titulo->setAccessibleName(
            QStringLiteral("Número do concurso %1").arg(concursoAtual_));
    }

    void animarOpacidade(QWidget* componente)
    {
        interromperAnimacoes(componente);
        auto* efeito = qobject_cast<QGraphicsOpacityEffect*>(componente->graphicsEffect());
        if (!efeito) {
            efeito = new QGraphicsOpacityEffect(componente);
            componente->setGraphicsEffect(efeito);
        }
        efeito->setOpacity(0.0);
        auto* animacao = new QPropertyAnimation(efeito, "opacity", componente);
        animacao->setDuration(DuracaoAnimacao);
        animacao->setStartValue(0.0);
        animacao->setEndValue(1.0);
        animacao->setEasingCurve(QEasingCurve::OutCubic);
        animacao->start(QAbstractAnimation::DeleteWhenStopped);
    }

    void interromperAnimacoes(QWidget* componente)
    {
        const auto animacoes = componente->findChildren<QPropertyAnimation*>();
        for (QPropertyAnimation* animacao : animacoes) {
            animacao->stop();
            animacao->deleteLater();
        }
    }

    void animarEntradaVertical(QWidget* componente)
    {
        interromperAnimacoes(componente);
        const QPoint final = componente->pos();
        auto* animacao = new QPropertyAnimation(componente, "pos", componente);
        animacao->setDuration(DuracaoAnimacao);
        animacao->setStartValue(final + QPoint(0, 10));
        animacao->setEndValue(final);
        animacao->setEasingCurve(QEasingCurve::OutCubic);
        animacao->start(QAbstractAnimation::DeleteWhenStopped);
    }

    void garantirCamadaModal()
    {
        if (camadaModal_) return;
        camadaModal_ = new QFrame(superficie_);
        camadaModal_->setObjectName(QStringLiteral("camadaModal"));
        camadaModal_->setFocusPolicy(Qt::StrongFocus);
        camadaModal_->hide();
    }

    void abrirModalCentral()
    {
        garantirCamadaModal();
        if (cartaoModal_) delete cartaoModal_;
        QPushButton* fechar = nullptr;
        cartaoModal_ = criarCartao(camadaModal_, tituloModal_, fechar);
        QObject::connect(fechar, &QPushButton::clicked, this, [this] { fecharTudo(); });
        atualizarTitulo(tituloModal_);
        camadaModal_->setGeometry(superficie_->rect());
        cartaoModal_->adjustSize();
        cartaoModal_->move((camadaModal_->width() - cartaoModal_->width()) / 2,
                           (camadaModal_->height() - cartaoModal_->height()) / 2);
        camadaModal_->show();
        camadaModal_->raise();
        animarOpacidade(camadaModal_);
        fechar->setFocus();
    }

    void abrirGavetaModal()
    {
        garantirCamadaModal();
        if (cartaoModal_) delete cartaoModal_;
        QPushButton* fechar = nullptr;
        cartaoModal_ = criarCartao(camadaModal_, tituloModal_, fechar);
        cartaoModal_->setMinimumWidth(360);
        QObject::connect(fechar, &QPushButton::clicked, this, [this] { fecharTudo(); });
        atualizarTitulo(tituloModal_);
        camadaModal_->setGeometry(superficie_->rect());
        camadaModal_->show();
        camadaModal_->raise();
        cartaoModal_->adjustSize();
        const QRect final(camadaModal_->width() - cartaoModal_->width() - 18,
                          18, cartaoModal_->width(),
                          camadaModal_->height() - 36);
        const QRect inicial(camadaModal_->width() + 10, 18,
                            final.width(), final.height());
        cartaoModal_->setGeometry(inicial);
        interromperAnimacoes(cartaoModal_);
        auto* animacao = new QPropertyAnimation(cartaoModal_, "geometry", cartaoModal_);
        animacao->setDuration(DuracaoAnimacao);
        animacao->setStartValue(inicial);
        animacao->setEndValue(final);
        animacao->setEasingCurve(QEasingCurve::OutCubic);
        animacao->start(QAbstractAnimation::DeleteWhenStopped);
        fechar->setFocus();
    }

    void garantirPopover()
    {
        if (popover_) return;
        QPushButton* fechar = nullptr;
        popover_ = criarCartao(superficie_, tituloPopover_, fechar);
        popover_->setMinimumWidth(320);
        QObject::connect(fechar, &QPushButton::clicked, this, [this] { fecharTudo(); });
        popover_->hide();
    }

    void abrirPopover()
    {
        garantirPopover();
        atualizarTitulo(tituloPopover_);
        popover_->adjustSize();
        popover_->show();
        popover_->raise();
        reposicionarPopover();
        animarEntradaVertical(popover_);
    }

    void reposicionarPopover()
    {
        if (!popover_ || !popover_->isVisible()
            || tipoAtual_ != TipoApresentacao::Popover) return;
        const QPoint ancora = visualizacao_->mapFromScene(posicaoPopover_);
        int x = ancora.x() + 45;
        int y = ancora.y() - popover_->height() / 2;
        if (x + popover_->width() > superficie_->width() - 12)
            x = ancora.x() - popover_->width() - 45;
        x = std::clamp(x, 12, std::max(12, superficie_->width() - popover_->width() - 12));
        y = std::clamp(y, 12, std::max(12, superficie_->height() - popover_->height() - 12));
        popover_->move(x, y);
    }

    void garantirInspetor()
    {
        if (inspetor_) return;
        QPushButton* fechar = nullptr;
        inspetor_ = criarCartao(superficie_, tituloInspetor_, fechar);
        inspetor_->setMinimumWidth(340);
        QObject::connect(fechar, &QPushButton::clicked, this, [this] { fecharTudo(); });
        inspetor_->hide();
    }

    void abrirInspetor()
    {
        garantirInspetor();
        atualizarTitulo(tituloInspetor_);
        inspetor_->show();
        inspetor_->raise();
        const QRect final(superficie_->width() - 360, 16, 344,
                          superficie_->height() - 32);
        const QRect inicial(superficie_->width() + 10, 16, 344, final.height());
        inspetor_->setGeometry(inicial);
        interromperAnimacoes(inspetor_);
        auto* animacao = new QPropertyAnimation(inspetor_, "geometry", inspetor_);
        animacao->setDuration(DuracaoAnimacao);
        animacao->setStartValue(inicial);
        animacao->setEndValue(final);
        animacao->setEasingCurve(QEasingCurve::OutCubic);
        animacao->start(QAbstractAnimation::DeleteWhenStopped);
    }

    void garantirPainelInferior()
    {
        if (painelInferior_) return;
        QPushButton* fechar = nullptr;
        painelInferior_ = criarCartao(superficie_, tituloPainelInferior_, fechar);
        QObject::connect(fechar, &QPushButton::clicked, this, [this] { fecharTudo(); });
        painelInferior_->hide();
    }

    void abrirPainelInferior()
    {
        garantirPainelInferior();
        atualizarTitulo(tituloPainelInferior_);
        painelInferior_->show();
        painelInferior_->raise();
        const int altura = 190;
        const QRect final(16, superficie_->height() - altura - 16,
                          superficie_->width() - 32, altura);
        const QRect inicial(16, superficie_->height() + 5, final.width(), altura);
        painelInferior_->setGeometry(inicial);
        interromperAnimacoes(painelInferior_);
        auto* animacao = new QPropertyAnimation(
            painelInferior_, "geometry", painelInferior_);
        animacao->setDuration(DuracaoAnimacao);
        animacao->setStartValue(inicial);
        animacao->setEndValue(final);
        animacao->setEasingCurve(QEasingCurve::OutCubic);
        animacao->start(QAbstractAnimation::DeleteWhenStopped);
    }

    void ajustarComponentes()
    {
        if (camadaModal_ && camadaModal_->isVisible()) {
            camadaModal_->setGeometry(superficie_->rect());
            if (tipoAtual_ == TipoApresentacao::ModalCentral && cartaoModal_)
                cartaoModal_->move((camadaModal_->width() - cartaoModal_->width()) / 2,
                                   (camadaModal_->height() - cartaoModal_->height()) / 2);
            if (tipoAtual_ == TipoApresentacao::GavetaModal && cartaoModal_)
                cartaoModal_->setGeometry(camadaModal_->width() - 378, 18, 360,
                                          camadaModal_->height() - 36);
        }
        if (inspetor_ && inspetor_->isVisible())
            inspetor_->setGeometry(superficie_->width() - 360, 16, 344,
                                   superficie_->height() - 32);
        if (painelInferior_ && painelInferior_->isVisible())
            painelInferior_->setGeometry(16, superficie_->height() - 206,
                                         superficie_->width() - 32, 190);
        reposicionarPopover();
    }

    VisualizacaoArvore* visualizacao_;
    QWidget* superficie_;
    int concursoAtual_ = 0;
    QPointF posicaoPopover_;
    std::optional<TipoApresentacao> tipoAtual_;
    QGraphicsItem* noOrigem_ = nullptr;
    QFrame* camadaModal_ = nullptr;
    QFrame* cartaoModal_ = nullptr;
    QLabel* tituloModal_ = nullptr;
    QFrame* popover_ = nullptr;
    QLabel* tituloPopover_ = nullptr;
    QFrame* inspetor_ = nullptr;
    QLabel* tituloInspetor_ = nullptr;
    QFrame* painelInferior_ = nullptr;
    QLabel* tituloPainelInferior_ = nullptr;
};

void adicionarConexoes(QGraphicsScene& cena, const NoArvore* no)
{
    if (!no) return;
    const auto adicionar = [&cena, no](const NoArvore* filho) {
        auto* linha = cena.addLine(QLineF(no->posicao, filho->posicao),
                                   QPen(QColor("#91a4b7"), 2));
        linha->setZValue(-1);
    };
    if (no->esquerdo) {
        adicionar(no->esquerdo.get());
        adicionarConexoes(cena, no->esquerdo.get());
    }
    if (no->direito) {
        adicionar(no->direito.get());
        adicionarConexoes(cena, no->direito.get());
    }
}

void adicionarNos(QGraphicsScene& cena, const NoArvore* no, int& indice,
                   const AcaoAoAtivar& acao)
{
    if (!no) return;
    const auto tipo = static_cast<TipoApresentacao>((indice / 2) + 1);
    cena.addItem(new ItemNo(no->valor, tipo, no->posicao, acao));
    ++indice;
    adicionarNos(cena, no->esquerdo.get(), indice, acao);
    adicionarNos(cena, no->direito.get(), indice, acao);
}

std::unique_ptr<NoArvore> criarArvoreDemonstracao()
{
    const std::vector<int> valores{52, 28, 76, 14, 39, 63, 88, 7, 21, 45};
    std::unique_ptr<NoArvore> raiz;
    for (int valor : valores) inserirNo(raiz, valor);
    int indice = 0;
    calcularPosicoes(raiz.get(), 0, indice);
    return raiz;
}

} // namespace

int main(int quantidadeArgumentos, char* argumentos[])
{
    QApplication aplicacao(quantidadeArgumentos, argumentos);
    aplicacao.setStyleSheet(QStringLiteral(R"(
        QMainWindow, QWidget#conteudoCentral { background: #eef3f8; }
        QLabel { color: #17324d; font-family: "Segoe UI"; }
        QPushButton { background: #ffffff; color: #17324d; border: 1px solid #cad6e2;
                      border-radius: 8px; padding: 7px 12px; font-weight: 600; }
        QPushButton:hover { background: #e8f3fa; border-color: #39728f; }
        QPushButton:focus { border: 2px solid #ffb020; }
        QFrame#camadaModal { background: rgba(15, 35, 55, 145); }
        QFrame#cartaoDetalhes { background: #ffffff; border: 1px solid #d9e3ec;
                               border-radius: 14px; }
        QLabel#rotuloSecao { color: #687d91; font-size: 10px; font-weight: 700; }
        QLabel#tituloConcurso { color: #132d46; font-size: 22px; font-weight: 700; }
        QLabel#marcadorDezena { background: #dff7ec; color: #087f5b;
                               border: 1px solid #9ce0c1; border-radius: 19px;
                               font-weight: 700; }
        QPushButton#botaoFechar { border: none; background: #edf2f6;
                                 border-radius: 16px; font-size: 20px; padding: 0; }
        QPushButton#botaoFechar:hover { background: #dfe8ef; }
    )"));

    auto arvore = criarArvoreDemonstracao();
    QGraphicsScene cena;
    cena.setItemIndexMethod(QGraphicsScene::BspTreeIndex);
    adicionarConexoes(cena, arvore.get());

    QMainWindow janela;
    janela.setWindowTitle(QStringLiteral("Laboratório de detalhes dos nós"));
    janela.resize(1180, 760);
    janela.setMinimumSize(800, 600);
    auto* central = new QWidget;
    central->setObjectName(QStringLiteral("conteudoCentral"));
    auto* principal = new QVBoxLayout(central);
    principal->setContentsMargins(18, 16, 18, 18);
    principal->setSpacing(12);
    auto* barra = new QHBoxLayout;
    auto* titulo = new QLabel(QStringLiteral("Apresentações de detalhes"));
    QFont fonteTitulo = titulo->font();
    fonteTitulo.setPointSize(17);
    fonteTitulo.setBold(true);
    titulo->setFont(fonteTitulo);
    auto* instrucao = new QLabel(QStringLiteral(
        "Selecione os nós para comparar 5 abordagens modernas"));
    auto* afastar = new QPushButton(QStringLiteral("−"));
    auto* enquadrar = new QPushButton(QStringLiteral("Ver tudo"));
    auto* aproximar = new QPushButton(QStringLiteral("+"));
    auto* visualizacao = new VisualizacaoArvore(&cena);
    barra->addWidget(titulo);
    barra->addSpacing(14);
    barra->addWidget(instrucao);
    barra->addStretch();
    barra->addWidget(afastar);
    barra->addWidget(enquadrar);
    barra->addWidget(aproximar);
    principal->addLayout(barra);
    principal->addWidget(visualizacao);
    janela.setCentralWidget(central);

    auto* apresentador = new ApresentadorDetalhes(visualizacao, &janela);
    const AcaoAoAtivar acao = [apresentador](
        int valor, TipoApresentacao tipo, const QPointF& posicao, const QPoint&) {
        apresentador->apresentar(valor, tipo, posicao);
    };
    int indice = 0;
    adicionarNos(cena, arvore.get(), indice, acao);
    cena.setSceneRect(cena.itemsBoundingRect().adjusted(-90, -90, 90, 90));

    QObject::connect(afastar, &QPushButton::clicked,
                     visualizacao, &VisualizacaoArvore::afastar);
    QObject::connect(enquadrar, &QPushButton::clicked,
                     visualizacao, &VisualizacaoArvore::enquadrarTudo);
    QObject::connect(aproximar, &QPushButton::clicked,
                     visualizacao, &VisualizacaoArvore::aproximar);

    janela.show();
    QTimer::singleShot(0, visualizacao, &VisualizacaoArvore::enquadrarTudo);
    return aplicacao.exec();
}
