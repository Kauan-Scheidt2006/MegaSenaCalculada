#pragma once

#include "interface/apresentacao/arvore/ModeloVisualArvore.hpp"

#include <QColor>
#include <QGraphicsView>

#include <map>
#include <optional>
#include <set>
#include <vector>

class QGraphicsScene;
class QKeyEvent;
class ItemNoArvore;
class ItemLigacaoArvore;

class VisualizacaoArvore final : public QGraphicsView
{
    Q_OBJECT
    Q_PROPERTY(qreal diametroNo READ diametroNo WRITE definirDiametroNo)
    Q_PROPERTY(qreal espacamentoHorizontal READ espacamentoHorizontal WRITE definirEspacamentoHorizontal)
    Q_PROPERTY(qreal espacamentoVertical READ espacamentoVertical WRITE definirEspacamentoVertical)
    Q_PROPERTY(qreal larguraLigacao READ larguraLigacao WRITE definirLarguraLigacao)
    Q_PROPERTY(qreal larguraBordaNo READ larguraBordaNo WRITE definirLarguraBordaNo)
    Q_PROPERTY(qreal larguraBordaNoFoco READ larguraBordaNoFoco WRITE definirLarguraBordaNoFoco)
    Q_PROPERTY(QColor corLigacao READ corLigacao WRITE definirCorLigacao)
    Q_PROPERTY(QColor corFundoNo READ corFundoNo WRITE definirCorFundoNo)
    Q_PROPERTY(QColor corBordaNo READ corBordaNo WRITE definirCorBordaNo)
    Q_PROPERTY(QColor corTextoNo READ corTextoNo WRITE definirCorTextoNo)
    Q_PROPERTY(QColor corFundoNoSelecionado READ corFundoNoSelecionado WRITE definirCorFundoNoSelecionado)
    Q_PROPERTY(QColor corBordaNoSelecionado READ corBordaNoSelecionado WRITE definirCorBordaNoSelecionado)
    Q_PROPERTY(QColor corTextoNoSelecionado READ corTextoNoSelecionado WRITE definirCorTextoNoSelecionado)
    Q_PROPERTY(QColor corFundoNoVermelho READ corFundoNoVermelho WRITE definirCorFundoNoVermelho)
    Q_PROPERTY(QColor corBordaNoVermelho READ corBordaNoVermelho WRITE definirCorBordaNoVermelho)
    Q_PROPERTY(QColor corTextoNoVermelho READ corTextoNoVermelho WRITE definirCorTextoNoVermelho)
    Q_PROPERTY(QColor corFundoNoVermelhoSelecionado READ corFundoNoVermelhoSelecionado WRITE definirCorFundoNoVermelhoSelecionado)
    Q_PROPERTY(QColor corBordaNoVermelhoSelecionado READ corBordaNoVermelhoSelecionado WRITE definirCorBordaNoVermelhoSelecionado)
    Q_PROPERTY(QColor corTextoNoVermelhoSelecionado READ corTextoNoVermelhoSelecionado WRITE definirCorTextoNoVermelhoSelecionado)
    Q_PROPERTY(qreal tamanhoFonteNumeroNo READ tamanhoFonteNumeroNo WRITE definirTamanhoFonteNumeroNo)
    Q_PROPERTY(qreal tamanhoFonteFrequenciaNo READ tamanhoFonteFrequenciaNo WRITE definirTamanhoFonteFrequenciaNo)

public:
    explicit VisualizacaoArvore(QWidget* pai = nullptr);

    void apresentar(const apresentacao::arvore::ModeloVisualArvore& modelo,
                    bool preservarViewport = false);
    void enquadrar();
    void aumentarZoom();
    void reduzirZoom();
    void definirZoom(qreal escala);
    [[nodiscard]] qreal zoomAtual() const noexcept;
    [[nodiscard]] qreal zoomMinimo() const noexcept;
    [[nodiscard]] qreal zoomMaximo() const noexcept;
    void definirModoArrasto(bool ativo);
    void definirModoFoco(bool ativo);
    void expandirTudo();
    void recolherOuExpandirSelecao();
    void centralizarEm(const QPointF& posicaoCena);
    [[nodiscard]] bool selecionarRegistro(
        const apresentacao::arvore::IdentificadorRegistro& identificador);
    [[nodiscard]] bool focarRegistro(
        const apresentacao::arvore::IdentificadorRegistro& identificador);
    [[nodiscard]] std::optional<QPoint> posicaoRegistroNaViewport(
        const apresentacao::arvore::IdentificadorRegistro& identificador) const;
    [[nodiscard]] std::optional<apresentacao::arvore::IdentificadorRegistro>
    identificadorSelecionado() const noexcept;
    [[nodiscard]] std::size_t quantidadeNos() const noexcept;

    [[nodiscard]] qreal diametroNo() const noexcept;
    void definirDiametroNo(qreal valor) noexcept;
    [[nodiscard]] qreal espacamentoHorizontal() const noexcept;
    void definirEspacamentoHorizontal(qreal valor) noexcept;
    [[nodiscard]] qreal espacamentoVertical() const noexcept;
    void definirEspacamentoVertical(qreal valor) noexcept;
    [[nodiscard]] qreal larguraLigacao() const noexcept;
    void definirLarguraLigacao(qreal valor) noexcept;
    [[nodiscard]] qreal larguraBordaNo() const noexcept;
    void definirLarguraBordaNo(qreal valor) noexcept;
    [[nodiscard]] qreal larguraBordaNoFoco() const noexcept;
    void definirLarguraBordaNoFoco(qreal valor) noexcept;
    [[nodiscard]] QColor corLigacao() const noexcept;
    void definirCorLigacao(const QColor& valor) noexcept;
    [[nodiscard]] QColor corFundoNo() const noexcept;
    void definirCorFundoNo(const QColor& valor) noexcept;
    [[nodiscard]] QColor corBordaNo() const noexcept;
    void definirCorBordaNo(const QColor& valor) noexcept;
    [[nodiscard]] QColor corTextoNo() const noexcept;
    void definirCorTextoNo(const QColor& valor) noexcept;
    [[nodiscard]] QColor corFundoNoSelecionado() const noexcept;
    void definirCorFundoNoSelecionado(const QColor& valor) noexcept;
    [[nodiscard]] QColor corBordaNoSelecionado() const noexcept;
    void definirCorBordaNoSelecionado(const QColor& valor) noexcept;
    [[nodiscard]] QColor corTextoNoSelecionado() const noexcept;
    void definirCorTextoNoSelecionado(const QColor& valor) noexcept;
    [[nodiscard]] QColor corFundoNoVermelho() const noexcept;
    void definirCorFundoNoVermelho(const QColor& valor) noexcept;
    [[nodiscard]] QColor corBordaNoVermelho() const noexcept;
    void definirCorBordaNoVermelho(const QColor& valor) noexcept;
    [[nodiscard]] QColor corTextoNoVermelho() const noexcept;
    void definirCorTextoNoVermelho(const QColor& valor) noexcept;
    [[nodiscard]] QColor corFundoNoVermelhoSelecionado() const noexcept;
    void definirCorFundoNoVermelhoSelecionado(const QColor& valor) noexcept;
    [[nodiscard]] QColor corBordaNoVermelhoSelecionado() const noexcept;
    void definirCorBordaNoVermelhoSelecionado(const QColor& valor) noexcept;
    [[nodiscard]] QColor corTextoNoVermelhoSelecionado() const noexcept;
    void definirCorTextoNoVermelhoSelecionado(const QColor& valor) noexcept;
    [[nodiscard]] qreal tamanhoFonteNumeroNo() const noexcept;
    void definirTamanhoFonteNumeroNo(qreal valor) noexcept;
    [[nodiscard]] qreal tamanhoFonteFrequenciaNo() const noexcept;
    void definirTamanhoFonteFrequenciaNo(qreal valor) noexcept;

signals:
    void registroAtivado(
        apresentacao::arvore::IdentificadorRegistro identificador);
    void zoomAlterado(int percentual);
    void viewportAlterado();

protected:
    void keyPressEvent(QKeyEvent* evento) override;
    void wheelEvent(QWheelEvent* evento) override;
    void resizeEvent(QResizeEvent* evento) override;

private:
    void atualizarContextoVisual();
    void atualizarVisibilidade();
    QGraphicsScene* cena_ = nullptr;
    std::map<apresentacao::arvore::IdentificadorRegistro, ItemNoArvore*> itensNo_;
    std::map<apresentacao::arvore::IdentificadorRegistro, ItemLigacaoArvore*> ligacoesPorFilho_;
    std::map<apresentacao::arvore::IdentificadorRegistro,
             apresentacao::arvore::IdentificadorRegistro> pais_;
    std::map<apresentacao::arvore::IdentificadorRegistro,
             std::vector<apresentacao::arvore::IdentificadorRegistro>> filhos_;
    std::set<apresentacao::arvore::IdentificadorRegistro> recolhidos_;
    std::optional<apresentacao::arvore::IdentificadorRegistro> raizAtual_;
    bool modoFoco_ = false;
    qreal diametroNo_ = 0;
    qreal espacamentoHorizontal_ = 0;
    qreal espacamentoVertical_ = 0;
    qreal larguraLigacao_ = 0;
    qreal larguraBordaNo_ = 0;
    qreal larguraBordaNoFoco_ = 0;
    QColor corLigacao_;
    QColor corFundoNo_;
    QColor corBordaNo_;
    QColor corTextoNo_;
    QColor corFundoNoSelecionado_;
    QColor corBordaNoSelecionado_;
    QColor corTextoNoSelecionado_;
    QColor corFundoNoVermelho_;
    QColor corBordaNoVermelho_;
    QColor corTextoNoVermelho_;
    QColor corFundoNoVermelhoSelecionado_;
    QColor corBordaNoVermelhoSelecionado_;
    QColor corTextoNoVermelhoSelecionado_;
    qreal tamanhoFonteNumeroNo_ = 0;
    qreal tamanhoFonteFrequenciaNo_ = 0;
};
