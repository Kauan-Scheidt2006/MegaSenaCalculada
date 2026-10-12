#pragma once

#include "interface/fundamentos/Barra.hpp"
#include "interface/apresentacao/arvore/IdentificadorRegistro.hpp"

class Botao;
class ControleZoom;
class QLineEdit;

class BarraFerramentasArvore final : public Barra
{
    Q_OBJECT

public:
    explicit BarraFerramentasArvore(QWidget* pai = nullptr);

    void definirPercentualZoom(int percentual);
    void definirRemocaoHabilitada(bool habilitada);
    [[nodiscard]] bool modoArrastoAtivo() const noexcept;

signals:
    void aumentoZoomSolicitado();
    void reducaoZoomSolicitada();
    void enquadramentoSolicitado();
    void modoArrastoAlterado(bool ativo);
    void modoFocoAlterado(bool ativo);
    void expansaoTotalSolicitada();
    void recolhimentoSelecaoSolicitado();
    void percursoEmOrdemSolicitado();
    void percursoPreOrdemSolicitado();
    void percursoPosOrdemSolicitado();
    void buscaSolicitada(
        apresentacao::arvore::IdentificadorRegistro identificador);
    void remocaoSolicitada(
        apresentacao::arvore::IdentificadorRegistro identificador);
    void buscaInvalida(const QString& mensagem);

private:
    void solicitarBusca();

    ControleZoom* controleZoom_ = nullptr;
    Botao* enquadrar_ = nullptr;
    Botao* arrastar_ = nullptr;
    Botao* foco_ = nullptr;
    Botao* expandirTudo_ = nullptr;
    Botao* recolherSelecao_ = nullptr;
    Botao* percorrerEmOrdem_ = nullptr;
    Botao* percorrerPreOrdem_ = nullptr;
    Botao* percorrerPosOrdem_ = nullptr;
    QLineEdit* chaveBusca_ = nullptr;
    Botao* buscar_ = nullptr;
    Botao* remover_ = nullptr;
};
