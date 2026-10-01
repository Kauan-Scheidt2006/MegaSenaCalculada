#include "interface/fundamentos/BaseContainer.hpp"

#include "interface/fundamentos/ConfiguradorComponente.hpp"

#include <QBoxLayout>
#include <QGridLayout>
#include <QStackedLayout>

#include <stdexcept>

BaseContainer::BaseContainer(const ParametrosContainer& parametros, QWidget* pai)
    : QWidget(pai), tipoDisposicao_(parametros.disposicao)
{
    ConfiguradorComponente::aplicar(*this, parametros.componente);
    disposicao_ = criarDisposicao(parametros);
    setLayout(disposicao_);
}

void BaseContainer::adicionarComponente(QWidget& componente, int proporcao,
                                        std::optional<Qt::Alignment> alinhamento)
{
    if (tipoDisposicao_ == TipoDisposicao::Vertical || tipoDisposicao_ == TipoDisposicao::Horizontal) {
        auto& disposicaoLinear = *static_cast<QBoxLayout*>(disposicao_);
        disposicaoLinear.addWidget(&componente, proporcao, alinhamento.value_or(Qt::Alignment{}));
        return;
    }

    if (tipoDisposicao_ == TipoDisposicao::Empilhada) {
        obterPilha().addWidget(&componente);
        return;
    }

    throw std::logic_error("Use adicionarComponenteNaGrade em containers com disposicao em grade.");
}

void BaseContainer::adicionarComponenteNaGrade(QWidget& componente, int linha, int coluna,
                                               int quantidadeLinhas, int quantidadeColunas,
                                               std::optional<Qt::Alignment> alinhamento)
{
    if (linha < 0 || coluna < 0 || quantidadeLinhas <= 0 || quantidadeColunas <= 0) {
        throw std::invalid_argument("A posicao e a extensao na grade devem ser validas.");
    }

    obterGrade().addWidget(&componente, linha, coluna, quantidadeLinhas, quantidadeColunas,
                           alinhamento.value_or(Qt::Alignment{}));
}

void BaseContainer::adicionarEspacamentoExpansivel(int proporcao)
{
    if (tipoDisposicao_ != TipoDisposicao::Vertical && tipoDisposicao_ != TipoDisposicao::Horizontal) {
        throw std::logic_error("Espacamento expansivel requer uma disposicao linear.");
    }

    static_cast<QBoxLayout*>(disposicao_)->addStretch(proporcao);
}

void BaseContainer::exibirComponente(QWidget& componente)
{
    QStackedLayout& pilha = obterPilha();
    if (pilha.indexOf(&componente) < 0) {
        throw std::invalid_argument("O componente nao pertence a este container empilhado.");
    }

    pilha.setCurrentWidget(&componente);
}

void BaseContainer::removerComponente(QWidget& componente)
{
    disposicao_->removeWidget(&componente);
    componente.setParent(nullptr);
}

TipoDisposicao BaseContainer::tipoDisposicao() const noexcept
{
    return tipoDisposicao_;
}

int BaseContainer::quantidadeComponentes() const noexcept
{
    return disposicao_->count();
}

QLayout* BaseContainer::criarDisposicao(const ParametrosContainer& parametros)
{
    QLayout* disposicao = nullptr;
    switch (parametros.disposicao) {
    case TipoDisposicao::Vertical:
        disposicao = new QVBoxLayout;
        break;
    case TipoDisposicao::Horizontal:
        disposicao = new QHBoxLayout;
        break;
    case TipoDisposicao::Grade:
        disposicao = new QGridLayout;
        break;
    case TipoDisposicao::Empilhada:
        disposicao = new QStackedLayout;
        break;
    }

    disposicao->setContentsMargins(parametros.margens);
    if (parametros.espacamento) {
        disposicao->setSpacing(*parametros.espacamento);
    }
    if (parametros.alinhamento) {
        disposicao->setAlignment(*parametros.alinhamento);
    }
    return disposicao;
}

QGridLayout& BaseContainer::obterGrade()
{
    if (tipoDisposicao_ != TipoDisposicao::Grade) {
        throw std::logic_error("A operacao requer uma disposicao em grade.");
    }
    return *static_cast<QGridLayout*>(disposicao_);
}

QStackedLayout& BaseContainer::obterPilha()
{
    if (tipoDisposicao_ != TipoDisposicao::Empilhada) {
        throw std::logic_error("A operacao requer uma disposicao empilhada.");
    }
    return *static_cast<QStackedLayout*>(disposicao_);
}
