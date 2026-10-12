#pragma once

#include "interface/apresentacao/arvore/DadosDetalhesRegistro.hpp"
#include "interface/fundamentos/Painel.hpp"

class Botao;
class CampoDetalhe;
class GrupoDezenasConcurso;
class Texto;

class PopoverDetalhesRegistro final : public Painel
{
    Q_OBJECT

public:
    explicit PopoverDetalhesRegistro(QWidget* pai = nullptr);

    void apresentar(const apresentacao::arvore::DadosDetalhesRegistro& detalhes);

signals:
    void fechamentoSolicitado();

protected:
    bool eventFilter(QObject* observado, QEvent* evento) override;
    void keyPressEvent(QKeyEvent* evento) override;

private:
    Texto* titulo_ = nullptr;
    Botao* fechar_ = nullptr;
    CampoDetalhe* dezena_ = nullptr;
    CampoDetalhe* frequencia_ = nullptr;
    CampoDetalhe* concurso_ = nullptr;
    CampoDetalhe* data_ = nullptr;
    GrupoDezenasConcurso* dezenas_ = nullptr;
};
