#include "interface/componentes/arvore/VisualizacaoArvore.hpp"

#include "interface/componentes/arvore/CalculadorDisposicaoArvore.hpp"
#include "interface/componentes/arvore/ItemLigacaoArvore.hpp"
#include "interface/componentes/arvore/ItemNoArvore.hpp"

#include <QGraphicsScene>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QScrollBar>
#include <QTimer>
#include <QWheelEvent>

#include <algorithm>
#include <cmath>
#include <queue>
#include <set>

namespace {

constexpr qreal ZoomMinimo = 0.25;
constexpr qreal ZoomMaximo = 4.0;
constexpr qreal FatorZoom = 1.2;

int percentualZoom(qreal escala)
{
    return std::max(1, static_cast<int>(std::lround(escala * 100.0)));
}

} // namespace

VisualizacaoArvore::VisualizacaoArvore(QWidget* pai)
    : QGraphicsView(pai), cena_(new QGraphicsScene(this))
{
    setObjectName(QStringLiteral("visualizacaoArvore"));
    setAccessibleName(QStringLiteral("Visualizacao da arvore selecionada"));
    setScene(cena_);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);
    setFocusPolicy(Qt::StrongFocus);
    connect(horizontalScrollBar(), &QScrollBar::valueChanged,
            this, &VisualizacaoArvore::viewportAlterado);
    connect(verticalScrollBar(), &QScrollBar::valueChanged,
            this, &VisualizacaoArvore::viewportAlterado);
}

void VisualizacaoArvore::apresentar(
    const apresentacao::arvore::ModeloVisualArvore& modelo,
    bool preservarViewport)
{
    const qreal zoomAnterior = zoomAtual();
    const QPointF centroAnterior = mapToScene(viewport()->rect().center());
    ensurePolished();
    cena_->clear();
    itensNo_.clear();
    ligacoesPorFilho_.clear();
    pais_.clear();
    filhos_.clear();
    recolhidos_.clear();
    raizAtual_ = modelo.raiz();

    const auto disposicao =
        interface_grafica::arvore::CalculadorDisposicaoArvore{}.calcular(
            modelo, {diametroNo_, espacamentoHorizontal_, espacamentoVertical_});
    cena_->setSceneRect(disposicao.limites());

    for (const auto& ligacao : modelo.ligacoes()) {
        auto* itemLigacao = new ItemLigacaoArvore(
            ligacao,
            QLineF(disposicao.posicao(ligacao.pai),
                   disposicao.posicao(ligacao.filho)),
            corLigacao_, larguraLigacao_);
        cena_->addItem(itemLigacao);
        ligacoesPorFilho_.emplace(ligacao.filho, itemLigacao);
        pais_.emplace(ligacao.filho, ligacao.pai);
        filhos_[ligacao.pai].push_back(ligacao.filho);
    }

    const EstiloItemNoArvore estilo{
        diametroNo_, larguraBordaNo_, larguraBordaNoFoco_,
        {corFundoNo_, corBordaNo_, corTextoNo_,
         corFundoNoSelecionado_, corBordaNoSelecionado_,
         corTextoNoSelecionado_},
        {corFundoNoVermelho_, corBordaNoVermelho_, corTextoNoVermelho_,
         corFundoNoVermelhoSelecionado_, corBordaNoVermelhoSelecionado_,
         corTextoNoVermelhoSelecionado_},
        tamanhoFonteNumeroNo_,
        tamanhoFonteFrequenciaNo_,
    };
    for (const auto& no : modelo.nos()) {
        auto* item = new ItemNoArvore(
            no.identificador, no.semanticaVisual, estilo,
            [this](apresentacao::arvore::IdentificadorRegistro identificador) {
                atualizarContextoVisual();
                emit registroAtivado(identificador);
            });
        item->setPos(disposicao.posicao(no.identificador));
        cena_->addItem(item);
        itensNo_.emplace(no.identificador, item);
    }

    setAccessibleDescription(
        QStringLiteral("%1 nos, altura %2. Use as setas para navegar, Home para ir a raiz, Ctrl mais Enter para recolher ou expandir descendentes e Enter ou Espaco para abrir detalhes.")
            .arg(modelo.metricas().quantidadeNos)
            .arg(modelo.metricas().altura));
    QTimer::singleShot(0, this, [this, preservarViewport,
                                zoomAnterior, centroAnterior] {
        definirZoom(preservarViewport ? zoomAnterior : 1.0);
        if (preservarViewport) {
            centerOn(centroAnterior);
        } else if (raizAtual_) {
            const auto raiz = itensNo_.find(*raizAtual_);
            if (raiz != itensNo_.end()) {
                centerOn(raiz->second);
            }
        }
        emit viewportAlterado();
    });
}

void VisualizacaoArvore::definirModoFoco(bool ativo)
{
    modoFoco_ = ativo;
    atualizarContextoVisual();
}

void VisualizacaoArvore::expandirTudo()
{
    recolhidos_.clear();
    atualizarVisibilidade();
}

void VisualizacaoArvore::atualizarContextoVisual()
{
    const auto selecionado = identificadorSelecionado();
    std::set<apresentacao::arvore::IdentificadorRegistro> caminho;
    std::set<apresentacao::arvore::IdentificadorRegistro> contexto;
    if (selecionado) {
        auto atual = *selecionado;
        caminho.insert(atual);
        contexto.insert(atual);
        auto pai = pais_.find(atual);
        while (pai != pais_.end()) {
            caminho.insert(pai->second);
            contexto.insert(pai->second);
            atual = pai->second;
            pai = pais_.find(atual);
        }
        std::queue<std::pair<apresentacao::arvore::IdentificadorRegistro, int>> pendentes;
        pendentes.push({*selecionado, 0});
        while (!pendentes.empty()) {
            const auto [no, profundidade] = pendentes.front();
            pendentes.pop();
            contexto.insert(no);
            if (profundidade >= 2) continue;
            const auto descendentes = filhos_.find(no);
            if (descendentes != filhos_.end()) {
                for (const auto& filho : descendentes->second) {
                    pendentes.push({filho, profundidade + 1});
                }
            }
        }
    }
    for (const auto& [identificador, item] : itensNo_) {
        item->definirAtenuado(modoFoco_ && selecionado
                             && !contexto.contains(identificador));
    }
    for (const auto& [filho, item] : ligacoesPorFilho_) {
        const bool noCaminho = selecionado && caminho.contains(filho);
        item->definirDestaque(noCaminho,
                             modoFoco_ && selecionado && !noCaminho);
    }
    viewport()->update();
}

void VisualizacaoArvore::atualizarVisibilidade()
{
    std::set<apresentacao::arvore::IdentificadorRegistro> ocultos;
    for (const auto& raizRecolhida : recolhidos_) {
        std::queue<apresentacao::arvore::IdentificadorRegistro> pendentes;
        const auto encontrados = filhos_.find(raizRecolhida);
        if (encontrados != filhos_.end()) {
            for (const auto& filho : encontrados->second) pendentes.push(filho);
        }
        while (!pendentes.empty()) {
            const auto atual = pendentes.front();
            pendentes.pop();
            if (!ocultos.insert(atual).second) continue;
            const auto descendentes = filhos_.find(atual);
            if (descendentes != filhos_.end()) {
                for (const auto& filho : descendentes->second) pendentes.push(filho);
            }
        }
    }
    for (const auto& [identificador, item] : itensNo_) {
        item->setVisible(!ocultos.contains(identificador));
    }
    for (const auto& [filho, item] : ligacoesPorFilho_) {
        item->setVisible(!ocultos.contains(filho));
    }
    if (!cena_->items().isEmpty()) {
        cena_->setSceneRect(cena_->itemsBoundingRect().adjusted(
            -diametroNo_, -diametroNo_, diametroNo_, diametroNo_));
    }
    atualizarContextoVisual();
    emit viewportAlterado();
}

void VisualizacaoArvore::recolherOuExpandirSelecao()
{
    const auto selecionado = identificadorSelecionado();
    if (!selecionado || !filhos_.contains(*selecionado)) return;
    if (recolhidos_.contains(*selecionado)) {
        recolhidos_.erase(*selecionado);
    } else {
        recolhidos_.insert(*selecionado);
    }
    atualizarVisibilidade();
}

void VisualizacaoArvore::enquadrar()
{
    if (!cena_->items().isEmpty()) {
        fitInView(cena_->sceneRect(), Qt::KeepAspectRatio);
        emit zoomAlterado(percentualZoom(zoomAtual()));
        emit viewportAlterado();
    }
}

void VisualizacaoArvore::aumentarZoom()
{
    definirZoom(zoomAtual() * FatorZoom);
}

void VisualizacaoArvore::reduzirZoom()
{
    definirZoom(zoomAtual() / FatorZoom);
}

void VisualizacaoArvore::definirZoom(qreal escala)
{
    const qreal escalaLimitada = std::clamp(escala, ZoomMinimo, ZoomMaximo);
    const QPointF centro = mapToScene(viewport()->rect().center());
    resetTransform();
    scale(escalaLimitada, escalaLimitada);
    centerOn(centro);
    emit zoomAlterado(percentualZoom(escalaLimitada));
    emit viewportAlterado();
}

qreal VisualizacaoArvore::zoomAtual() const noexcept
{
    return transform().m11();
}

qreal VisualizacaoArvore::zoomMinimo() const noexcept
{
    return ZoomMinimo;
}

qreal VisualizacaoArvore::zoomMaximo() const noexcept
{
    return ZoomMaximo;
}

void VisualizacaoArvore::definirModoArrasto(bool ativo)
{
    setDragMode(ativo ? QGraphicsView::ScrollHandDrag
                      : QGraphicsView::NoDrag);
}

void VisualizacaoArvore::centralizarEm(const QPointF& posicaoCena)
{
    centerOn(posicaoCena);
    emit viewportAlterado();
}

bool VisualizacaoArvore::selecionarRegistro(
    const apresentacao::arvore::IdentificadorRegistro& identificador)
{
    const auto item = itensNo_.find(identificador);
    if (item == itensNo_.end()) {
        return false;
    }
    const bool focado = focarRegistro(identificador);
    Q_ASSERT(focado);
    centerOn(item->second);
    return true;
}

bool VisualizacaoArvore::focarRegistro(
    const apresentacao::arvore::IdentificadorRegistro& identificador)
{
    const auto item = itensNo_.find(identificador);
    if (item == itensNo_.end()) return false;
    setFocus(Qt::OtherFocusReason);
    cena_->clearSelection();
    item->second->setSelected(true);
    item->second->setFocus(Qt::OtherFocusReason);
    atualizarContextoVisual();
    return true;
}

std::optional<QPoint> VisualizacaoArvore::posicaoRegistroNaViewport(
    const apresentacao::arvore::IdentificadorRegistro& identificador) const
{
    const auto item = itensNo_.find(identificador);
    if (item == itensNo_.end()) return std::nullopt;
    return mapFromScene(item->second->scenePos());
}

std::optional<apresentacao::arvore::IdentificadorRegistro>
VisualizacaoArvore::identificadorSelecionado() const noexcept
{
    for (QGraphicsItem* item : cena_->selectedItems()) {
        if (const auto* no = dynamic_cast<ItemNoArvore*>(item)) {
            return no->identificador();
        }
    }
    return std::nullopt;
}

std::size_t VisualizacaoArvore::quantidadeNos() const noexcept
{
    return itensNo_.size();
}

qreal VisualizacaoArvore::diametroNo() const noexcept { return diametroNo_; }
void VisualizacaoArvore::definirDiametroNo(qreal valor) noexcept { diametroNo_ = valor; }
qreal VisualizacaoArvore::espacamentoHorizontal() const noexcept { return espacamentoHorizontal_; }
void VisualizacaoArvore::definirEspacamentoHorizontal(qreal valor) noexcept { espacamentoHorizontal_ = valor; }
qreal VisualizacaoArvore::espacamentoVertical() const noexcept { return espacamentoVertical_; }
void VisualizacaoArvore::definirEspacamentoVertical(qreal valor) noexcept { espacamentoVertical_ = valor; }
qreal VisualizacaoArvore::larguraLigacao() const noexcept { return larguraLigacao_; }
void VisualizacaoArvore::definirLarguraLigacao(qreal valor) noexcept { larguraLigacao_ = valor; }
qreal VisualizacaoArvore::larguraBordaNo() const noexcept { return larguraBordaNo_; }
void VisualizacaoArvore::definirLarguraBordaNo(qreal valor) noexcept { larguraBordaNo_ = valor; }
qreal VisualizacaoArvore::larguraBordaNoFoco() const noexcept { return larguraBordaNoFoco_; }
void VisualizacaoArvore::definirLarguraBordaNoFoco(qreal valor) noexcept { larguraBordaNoFoco_ = valor; }
QColor VisualizacaoArvore::corLigacao() const noexcept { return corLigacao_; }
void VisualizacaoArvore::definirCorLigacao(const QColor& valor) noexcept { corLigacao_ = valor; }
QColor VisualizacaoArvore::corFundoNo() const noexcept { return corFundoNo_; }
void VisualizacaoArvore::definirCorFundoNo(const QColor& valor) noexcept { corFundoNo_ = valor; }
QColor VisualizacaoArvore::corBordaNo() const noexcept { return corBordaNo_; }
void VisualizacaoArvore::definirCorBordaNo(const QColor& valor) noexcept { corBordaNo_ = valor; }
QColor VisualizacaoArvore::corTextoNo() const noexcept { return corTextoNo_; }
void VisualizacaoArvore::definirCorTextoNo(const QColor& valor) noexcept { corTextoNo_ = valor; }
QColor VisualizacaoArvore::corFundoNoSelecionado() const noexcept { return corFundoNoSelecionado_; }
void VisualizacaoArvore::definirCorFundoNoSelecionado(const QColor& valor) noexcept { corFundoNoSelecionado_ = valor; }
QColor VisualizacaoArvore::corBordaNoSelecionado() const noexcept { return corBordaNoSelecionado_; }
void VisualizacaoArvore::definirCorBordaNoSelecionado(const QColor& valor) noexcept { corBordaNoSelecionado_ = valor; }
QColor VisualizacaoArvore::corTextoNoSelecionado() const noexcept { return corTextoNoSelecionado_; }
void VisualizacaoArvore::definirCorTextoNoSelecionado(const QColor& valor) noexcept { corTextoNoSelecionado_ = valor; }
QColor VisualizacaoArvore::corFundoNoVermelho() const noexcept { return corFundoNoVermelho_; }
void VisualizacaoArvore::definirCorFundoNoVermelho(const QColor& valor) noexcept { corFundoNoVermelho_ = valor; }
QColor VisualizacaoArvore::corBordaNoVermelho() const noexcept { return corBordaNoVermelho_; }
void VisualizacaoArvore::definirCorBordaNoVermelho(const QColor& valor) noexcept { corBordaNoVermelho_ = valor; }
QColor VisualizacaoArvore::corTextoNoVermelho() const noexcept { return corTextoNoVermelho_; }
void VisualizacaoArvore::definirCorTextoNoVermelho(const QColor& valor) noexcept { corTextoNoVermelho_ = valor; }
QColor VisualizacaoArvore::corFundoNoVermelhoSelecionado() const noexcept { return corFundoNoVermelhoSelecionado_; }
void VisualizacaoArvore::definirCorFundoNoVermelhoSelecionado(const QColor& valor) noexcept { corFundoNoVermelhoSelecionado_ = valor; }
QColor VisualizacaoArvore::corBordaNoVermelhoSelecionado() const noexcept { return corBordaNoVermelhoSelecionado_; }
void VisualizacaoArvore::definirCorBordaNoVermelhoSelecionado(const QColor& valor) noexcept { corBordaNoVermelhoSelecionado_ = valor; }
QColor VisualizacaoArvore::corTextoNoVermelhoSelecionado() const noexcept { return corTextoNoVermelhoSelecionado_; }
void VisualizacaoArvore::definirCorTextoNoVermelhoSelecionado(const QColor& valor) noexcept { corTextoNoVermelhoSelecionado_ = valor; }
qreal VisualizacaoArvore::tamanhoFonteNumeroNo() const noexcept { return tamanhoFonteNumeroNo_; }
void VisualizacaoArvore::definirTamanhoFonteNumeroNo(qreal valor) noexcept { tamanhoFonteNumeroNo_ = valor; }
qreal VisualizacaoArvore::tamanhoFonteFrequenciaNo() const noexcept { return tamanhoFonteFrequenciaNo_; }
void VisualizacaoArvore::definirTamanhoFonteFrequenciaNo(qreal valor) noexcept { tamanhoFonteFrequenciaNo_ = valor; }

void VisualizacaoArvore::wheelEvent(QWheelEvent* evento)
{
    if (evento->angleDelta().y() > 0) {
        aumentarZoom();
    } else if (evento->angleDelta().y() < 0) {
        reduzirZoom();
    }
    evento->accept();
}

void VisualizacaoArvore::keyPressEvent(QKeyEvent* evento)
{
    const auto selecionado = identificadorSelecionado();
    if (evento->key() == Qt::Key_Home && raizAtual_) {
        (void)selecionarRegistro(*raizAtual_);
        evento->accept();
        return;
    }
    if (evento->key() == Qt::Key_Return
        && evento->modifiers().testFlag(Qt::ControlModifier)) {
        recolherOuExpandirSelecao();
        evento->accept();
        return;
    }
    if (selecionado && evento->key() == Qt::Key_Up) {
        const auto pai = pais_.find(*selecionado);
        if (pai != pais_.end()) (void)selecionarRegistro(pai->second);
        evento->accept();
        return;
    }
    if (selecionado && evento->key() == Qt::Key_Down) {
        const auto descendentes = filhos_.find(*selecionado);
        if (descendentes != filhos_.end() && !descendentes->second.empty()) {
            (void)selecionarRegistro(descendentes->second.front());
        }
        evento->accept();
        return;
    }
    if (selecionado && (evento->key() == Qt::Key_Left
                        || evento->key() == Qt::Key_Right)) {
        const auto pai = pais_.find(*selecionado);
        if (pai != pais_.end()) {
            const auto irmaos = filhos_.find(pai->second);
            if (irmaos != filhos_.end() && irmaos->second.size() == 2) {
                const auto& destino = irmaos->second.front() == *selecionado
                    ? irmaos->second.back() : irmaos->second.front();
                (void)selecionarRegistro(destino);
            }
        }
        evento->accept();
        return;
    }
    QGraphicsView::keyPressEvent(evento);
}

void VisualizacaoArvore::resizeEvent(QResizeEvent* evento)
{
    QGraphicsView::resizeEvent(evento);
    emit viewportAlterado();
}
