#pragma once

#include "interface/fundamentos/BaseContainer.hpp"

class QToolButton;
class QScrollArea;
class Texto;

class ResultadoOperacaoArvore final : public BaseContainer
{
    Q_OBJECT

public:
    explicit ResultadoOperacaoArvore(
        const QString& nomeObjeto,
        const QString& nomeResumo,
        const QString& nomeDetalhes,
        const QString& nomeRolagem,
        QWidget* pai = nullptr);

    void definirResultado(const QString& resumo, const QString& detalhes);
    void definirMensagem(const QString& mensagem);
    void definirExpandido(bool expandido);
    [[nodiscard]] bool expandido() const noexcept;

protected:
    void resizeEvent(QResizeEvent* evento) override;

private:
    void atualizarEstado(bool expandido);
    void atualizarAlturaDetalhes();

    QToolButton* resumo_ = nullptr;
    QScrollArea* rolagem_ = nullptr;
    Texto* detalhes_ = nullptr;
};
