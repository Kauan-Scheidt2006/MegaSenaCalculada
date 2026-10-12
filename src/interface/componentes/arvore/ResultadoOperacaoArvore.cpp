#include "interface/componentes/arvore/ResultadoOperacaoArvore.hpp"

#include "interface/componentes/basicos/Texto.hpp"

#include <QSizePolicy>
#include <QToolButton>
#include <QLayout>
#include <QResizeEvent>
#include <QScrollArea>
#include <QScrollBar>

#include <algorithm>

ResultadoOperacaoArvore::ResultadoOperacaoArvore(
    const QString& nomeObjeto,
    const QString& nomeResumo,
    const QString& nomeDetalhes,
    const QString& nomeRolagem,
    QWidget* pai)
    : BaseContainer(
          {.componente = {
               .nomeObjeto = nomeObjeto,
               .nomeAcessivel = QStringLiteral("Resultado da operação na árvore"),
               .politicaTamanho = QSizePolicy(
                   QSizePolicy::Expanding, QSizePolicy::Maximum),
               .fundoEstilizado = true},
           .disposicao = TipoDisposicao::Vertical,
           .margens = {0, 0, 0, 0},
           .espacamento = 0},
          pai),
      resumo_(new QToolButton(this)),
      rolagem_(new QScrollArea(this)),
      detalhes_(new Texto(QString{},
          {.nomeObjeto = nomeDetalhes,
           .nomeAcessivel = QStringLiteral("Vetor completo do percurso"),
           .politicaTamanho = QSizePolicy(
               QSizePolicy::Expanding, QSizePolicy::Preferred),
           .fundoEstilizado = false}, rolagem_))
{
    resumo_->setObjectName(nomeResumo);
    resumo_->setAccessibleName(QStringLiteral("Expandir resultado da operação"));
    resumo_->setCheckable(true);
    resumo_->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    resumo_->setArrowType(Qt::RightArrow);
    resumo_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    detalhes_->setWordWrap(true);
    detalhes_->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    detalhes_->setTextInteractionFlags(Qt::TextSelectableByMouse);
    rolagem_->setObjectName(nomeRolagem);
    rolagem_->setAccessibleName(
        QStringLiteral("Rolagem do vetor completo do percurso"));
    rolagem_->setWidgetResizable(true);
    rolagem_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    rolagem_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    rolagem_->setFrameShape(QFrame::NoFrame);
    rolagem_->setWidget(detalhes_);
    rolagem_->hide();

    adicionarComponente(*resumo_);
    adicionarComponente(*rolagem_);
    connect(resumo_, &QToolButton::toggled,
            this, &ResultadoOperacaoArvore::atualizarEstado);
}

void ResultadoOperacaoArvore::definirResultado(
    const QString& resumo, const QString& detalhes)
{
    resumo_->setText(resumo);
    detalhes_->definirConteudo(detalhes);
    resumo_->setCheckable(true);
    resumo_->setAccessibleName(
        QStringLiteral("Expandir ou recolher resultado: %1").arg(resumo));
    definirExpandido(false);
    atualizarAlturaDetalhes();
}

void ResultadoOperacaoArvore::definirMensagem(const QString& mensagem)
{
    resumo_->setText(mensagem);
    resumo_->setChecked(false);
    resumo_->setCheckable(false);
    resumo_->setArrowType(Qt::NoArrow);
    detalhes_->clear();
    detalhes_->setMinimumHeight(0);
    rolagem_->hide();
    updateGeometry();
}

void ResultadoOperacaoArvore::definirExpandido(bool expandido)
{
    if (!resumo_->isCheckable()) {
        return;
    }
    resumo_->setChecked(expandido);
    atualizarEstado(expandido);
}

bool ResultadoOperacaoArvore::expandido() const noexcept
{
    return resumo_->isChecked();
}

void ResultadoOperacaoArvore::atualizarEstado(bool expandido)
{
    resumo_->setArrowType(expandido ? Qt::DownArrow : Qt::RightArrow);
    rolagem_->setVisible(expandido);
    atualizarAlturaDetalhes();
    resumo_->setAccessibleDescription(
        expandido ? QStringLiteral("Expandido")
                  : QStringLiteral("Recolhido"));
    rolagem_->updateGeometry();
    updateGeometry();
    if (layout()) {
        layout()->activate();
    }
}

void ResultadoOperacaoArvore::resizeEvent(QResizeEvent* evento)
{
    BaseContainer::resizeEvent(evento);
    atualizarAlturaDetalhes();
}

void ResultadoOperacaoArvore::atualizarAlturaDetalhes()
{
    if (!rolagem_->isVisible()) {
        detalhes_->setMinimumHeight(0);
        rolagem_->setMinimumHeight(0);
        rolagem_->setMaximumHeight(0);
        return;
    }
    const int larguraBarra = rolagem_->verticalScrollBar()->sizeHint().width();
    const int larguraDisponivel = std::max(1, width() - larguraBarra - 24);
    const int alturaConteudo = std::max(
        0, detalhes_->heightForWidth(larguraDisponivel));
    detalhes_->setMinimumHeight(alturaConteudo);
    const int alturaVisivel = std::clamp(alturaConteudo + 4, 72, 280);
    rolagem_->setMinimumHeight(alturaVisivel);
    rolagem_->setMaximumHeight(alturaVisivel);
}
