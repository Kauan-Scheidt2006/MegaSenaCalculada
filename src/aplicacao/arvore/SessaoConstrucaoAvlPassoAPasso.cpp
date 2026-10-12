#include "aplicacao/arvore/SessaoConstrucaoAvlPassoAPasso.hpp"

#include <algorithm>
#include <utility>

namespace aplicacao::arvore {

SessaoConstrucaoAvlPassoAPasso::SessaoConstrucaoAvlPassoAPasso(
    std::vector<megasena::Registro> registros)
    : registros_(std::move(registros))
{
    reiniciar();
}

void SessaoConstrucaoAvlPassoAPasso::reiniciar()
{
    comparacoes_ = std::make_shared<std::size_t>(0);
    arvore_ = std::make_unique<Arvore>(
        detalhes::ComparadorRegistroContabilizado{comparacoes_});
    proximoRegistro_ = 0;
    numeroPasso_ = 0;
}

ResultadoPassoAvl SessaoConstrucaoAvlPassoAPasso::avancar()
{
    if (concluida()) {
        return criarResultado(QStringLiteral("Construção concluída."), true);
    }

    if (!arvore_->insercaoEmAndamento()) {
        arvore_->prepararInsercao(registros_[proximoRegistro_]);
    }

    const auto evento = arvore_->avancar();
    ++numeroPasso_;
    auto descricao = descrever(evento);
    if (evento.tipo == estruturas::TipoEventoPassoAvl::InsercaoConcluida) {
        ++proximoRegistro_;
    }
    return criarResultado(std::move(descricao), concluida());
}

ResultadoPassoAvl SessaoConstrucaoAvlPassoAPasso::estadoInicial() const
{
    return criarResultado(
        registros_.empty()
            ? QStringLiteral("Não há registros para construir a AVL.")
            : QStringLiteral("Árvore vazia. Acione Próximo passo para iniciar."),
        registros_.empty());
}

bool SessaoConstrucaoAvlPassoAPasso::concluida() const noexcept
{
    return proximoRegistro_ >= registros_.size()
        && !arvore_->insercaoEmAndamento();
}

ResultadoPassoAvl SessaoConstrucaoAvlPassoAPasso::criarResultado(
    QString descricao, bool concluido) const
{
    return {
        detalhes::criarModeloVisual(*arvore_, *comparacoes_,
                                    arvore_->quantidadeRotacoes()),
        std::move(descricao),
        std::min(proximoRegistro_ + (concluido ? 0U : 1U), registros_.size()),
        registros_.size(),
        numeroPasso_,
        concluido,
    };
}

QString SessaoConstrucaoAvlPassoAPasso::descrever(
    const estruturas::EventoPassoAvl<megasena::Registro>& evento) const
{
    const auto numeroDoEvento = [&evento]() {
        return evento.valor ? evento.valor->chave.numero : 0;
    };
    const auto numeroEmInsercao = [this]() {
        return proximoRegistro_ < registros_.size()
            ? registros_[proximoRegistro_].chave.numero
            : 0;
    };
    const auto numeroDaRaizAntes = [&evento]() {
        return evento.raizSubarvoreAntes
            ? evento.raizSubarvoreAntes->chave.numero
            : 0;
    };
    const auto numeroDaRaizDepois = [&evento]() {
        return evento.raizSubarvoreDepois
            ? evento.raizSubarvoreDepois->chave.numero
            : 0;
    };

    switch (evento.tipo) {
    case estruturas::TipoEventoPassoAvl::DescidaEsquerda:
        return QStringLiteral(
                   "Chamada atual: inserirNaSubarvore(no = %2, valor = %1). "
                   "Como %1 é menor que %2, o algoritmo recursivo chamaria "
                   "inserirNaSubarvore(no->esquerdo, valor). Na versão passo "
                   "a passo, caminho_ guarda a chamada atual e ligacaoAtual_ "
                   "passa a representar o filho esquerdo; valorPendente_ "
                   "continua sendo %1.")
            .arg(numeroEmInsercao())
            .arg(numeroDoEvento());
    case estruturas::TipoEventoPassoAvl::DescidaDireita:
        return QStringLiteral(
                   "Chamada atual: inserirNaSubarvore(no = %2, valor = %1). "
                   "Como %1 é maior que %2, o algoritmo recursivo chamaria "
                   "inserirNaSubarvore(no->direito, valor). Na versão passo "
                   "a passo, caminho_ guarda a chamada atual e ligacaoAtual_ "
                   "passa a representar o filho direito; valorPendente_ "
                   "continua sendo %1.")
            .arg(numeroEmInsercao())
            .arg(numeroDoEvento());
    case estruturas::TipoEventoPassoAvl::NoCriado:
        return QStringLiteral(
                   "Caso base: inserirNaSubarvore(no vazio, valor = %1). Um "
                   "novo nó foi criado e a inserção foi marcada como "
                   "realizada. A descida terminou; agora caminho_ será "
                   "percorrido no sentido inverso, simulando o retorno das "
                   "chamadas recursivas.")
            .arg(numeroDoEvento());
    case estruturas::TipoEventoPassoAvl::AlturaAtualizada:
        return QStringLiteral(
                   "Retorno da chamada que recebeu no = %2 e valor = %1. "
                   "Na versão recursiva, a execução volta para esse nó e "
                   "faz no = rebalancear(no). Na versão passo a passo, esse "
                   "nó foi recuperado de caminho_: sua altura foi atualizada "
                   "e seu fator de balanceamento não exige rotação.")
            .arg(numeroEmInsercao())
            .arg(numeroDoEvento());
    case estruturas::TipoEventoPassoAvl::RotacaoEsquerda:
        return QStringLiteral(
                   "Durante o retorno da inserção de %1, rebalancear(no = %2) "
                   "detectou desequilíbrio e executou uma rotação à esquerda. "
                   "A função retornaria a nova raiz %3; na versão passo a "
                   "passo, a ligação armazenada foi atualizada para apontar "
                   "para essa nova raiz.")
            .arg(numeroEmInsercao())
            .arg(numeroDaRaizAntes())
            .arg(numeroDaRaizDepois());
    case estruturas::TipoEventoPassoAvl::RotacaoDireita:
        return QStringLiteral(
                   "Durante o retorno da inserção de %1, rebalancear(no = %2) "
                   "detectou desequilíbrio e executou uma rotação à direita. "
                   "A função retornaria a nova raiz %3; na versão passo a "
                   "passo, a ligação armazenada foi atualizada para apontar "
                   "para essa nova raiz.")
            .arg(numeroEmInsercao())
            .arg(numeroDaRaizAntes())
            .arg(numeroDaRaizDepois());
    case estruturas::TipoEventoPassoAvl::InsercaoConcluida:
        if (proximoRegistro_ + 1U >= registros_.size()) {
            return QStringLiteral(
                       "A inserção de %1 retornou até a chamada inicial: "
                       "caminho_ está vazio e não há mais chamadas recursivas "
                       "simuladas para retomar. A árvore está balanceada e a "
                       "construção da AVL foi concluída.")
                .arg(numeroEmInsercao());
        }
        return QStringLiteral(
                   "A inserção de %1 retornou até a chamada inicial: "
                   "caminho_ está vazio e não há mais chamadas recursivas "
                   "simuladas para retomar. A árvore está balanceada e pronta "
                   "para iniciar a inserção do próximo registro.")
            .arg(numeroEmInsercao());
    }
    return {};
}

} // namespace aplicacao::arvore
