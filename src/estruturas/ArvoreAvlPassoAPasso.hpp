#pragma once

#include "estruturas/NoAvl.hpp"

#include <algorithm>
#include <cstddef>
#include <functional>
#include <memory>
#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>

namespace estruturas {

enum class TipoEventoPassoAvl {
    DescidaEsquerda,
    DescidaDireita,
    NoCriado,
    AlturaAtualizada,
    RotacaoEsquerda,
    RotacaoDireita,
    InsercaoConcluida
};

template<typename Valor>
struct EventoPassoAvl {
    TipoEventoPassoAvl tipo;
    std::optional<Valor> valor;
    std::optional<Valor> raizSubarvoreAntes;
    std::optional<Valor> raizSubarvoreDepois;
};

/**
 * AVL didática cuja pilha de inserção é representada explicitamente.
 * Cada chamada a avancar realiza uma única transição observável e nunca
 * espera pela interface.
 */
template<typename Valor, typename Comparador = std::less<Valor>>
class ArvoreAvlPassoAPasso final {
private:
    using No = NoAvl<Valor>;
    using Ligacao = std::unique_ptr<No>*;

    enum class Fase { Ociosa, Descendo, Retornando, RotacaoPendente };
    enum class Rotacao { Esquerda, Direita };

public:
    ArvoreAvlPassoAPasso() = default;
    explicit ArvoreAvlPassoAPasso(Comparador comparador)
        : comparador_(std::move(comparador)) {}

    void prepararInsercao(const Valor& valor)
    {
        if (insercaoEmAndamento()) {
            throw std::logic_error("Ja existe uma insercao AVL em andamento.");
        }
        valorPendente_ = valor;
        ligacaoAtual_ = &raiz_;
        caminho_.clear();
        fase_ = Fase::Descendo;
    }

    [[nodiscard]] EventoPassoAvl<Valor> avancar()
    {
        if (!insercaoEmAndamento()) {
            throw std::logic_error("Nao existe uma insercao AVL preparada.");
        }

        if (fase_ == Fase::Descendo) return avancarDescida();
        if (fase_ == Fase::Retornando) return avancarRetorno();
        if (fase_ == Fase::RotacaoPendente) return concluirRotacaoDupla();

        throw std::logic_error("Estado interno invalido na AVL passo a passo.");
    }

    [[nodiscard]] bool insercaoEmAndamento() const noexcept
    {
        return fase_ != Fase::Ociosa;
    }

    [[nodiscard]] bool vazia() const noexcept { return !raiz_; }
    [[nodiscard]] std::size_t quantidade() const noexcept { return quantidade_; }
    [[nodiscard]] std::size_t altura() const noexcept { return alturaDoNo(raiz_); }
    [[nodiscard]] std::size_t quantidadeRotacoes() const noexcept
    {
        return quantidadeRotacoes_;
    }

    template<typename Visitante>
    void visitarNosEmOrdem(Visitante visitante) const
    {
        visitarNosEmOrdem(raiz_.get(), nullptr, false, 0, visitante);
    }

private:
    [[nodiscard]] EventoPassoAvl<Valor> avancarDescida()
    {
        if (!*ligacaoAtual_) {
            *ligacaoAtual_ = std::make_unique<No>(*valorPendente_);
            ++quantidade_;
            fase_ = Fase::Retornando;
            return {TipoEventoPassoAvl::NoCriado, (*ligacaoAtual_)->valor,
                    std::nullopt, std::nullopt};
        }   

        No& no = **ligacaoAtual_;
        caminho_.push_back(ligacaoAtual_);
        if (comparador_(*valorPendente_, no.valor)) {
            ligacaoAtual_ = &no.esquerdo;
            return {TipoEventoPassoAvl::DescidaEsquerda, no.valor,
                    std::nullopt, std::nullopt};
        }
        if (comparador_(no.valor, *valorPendente_)) {
            ligacaoAtual_ = &no.direito;
            return {TipoEventoPassoAvl::DescidaDireita, no.valor,
                    std::nullopt, std::nullopt};
        }

        caminho_.clear();
        return concluirInsercao(no.valor);
    }

    [[nodiscard]] EventoPassoAvl<Valor> avancarRetorno()
    {
        if (caminho_.empty()) {
            return concluirInsercao(std::nullopt);
        }

        Ligacao ligacao = caminho_.back();
        caminho_.pop_back();
        No& no = **ligacao;
        atualizarAltura(no);
        const int fator = fatorBalanceamento(no);
        const Valor antes = no.valor;

        if (fator > 1) {
            if (fatorBalanceamento(*no.esquerdo) < 0) {
                const Valor raizFilhoAntes = no.esquerdo->valor;
                no.esquerdo = rotacionarEsquerda(std::move(no.esquerdo));
                const Valor raizFilhoDepois = no.esquerdo->valor;
                ligacaoRotacaoPendente_ = ligacao;
                rotacaoPendente_ = Rotacao::Direita;
                fase_ = Fase::RotacaoPendente;
                return {TipoEventoPassoAvl::RotacaoEsquerda,
                        raizFilhoDepois, raizFilhoAntes, raizFilhoDepois};
            }
            *ligacao = rotacionarDireita(std::move(*ligacao));
            return {TipoEventoPassoAvl::RotacaoDireita,
                    (*ligacao)->valor, antes, (*ligacao)->valor};
        }

        if (fator < -1) {
            if (fatorBalanceamento(*no.direito) > 0) {
                const Valor raizFilhoAntes = no.direito->valor;
                no.direito = rotacionarDireita(std::move(no.direito));
                const Valor raizFilhoDepois = no.direito->valor;
                ligacaoRotacaoPendente_ = ligacao;
                rotacaoPendente_ = Rotacao::Esquerda;
                fase_ = Fase::RotacaoPendente;
                return {TipoEventoPassoAvl::RotacaoDireita,
                        raizFilhoDepois, raizFilhoAntes, raizFilhoDepois};
            }
            *ligacao = rotacionarEsquerda(std::move(*ligacao));
            return {TipoEventoPassoAvl::RotacaoEsquerda,
                    (*ligacao)->valor, antes, (*ligacao)->valor};
        }

        return {TipoEventoPassoAvl::AlturaAtualizada, no.valor,
                std::nullopt, std::nullopt};
    }

    [[nodiscard]] EventoPassoAvl<Valor> concluirRotacaoDupla()
    {
        Ligacao ligacao = ligacaoRotacaoPendente_;
        const Valor antes = (*ligacao)->valor;
        const Rotacao rotacao = rotacaoPendente_;
        if (rotacao == Rotacao::Esquerda) {
            *ligacao = rotacionarEsquerda(std::move(*ligacao));
        } else {
            *ligacao = rotacionarDireita(std::move(*ligacao));
        }
        fase_ = Fase::Retornando;
        return {rotacao == Rotacao::Esquerda
                    ? TipoEventoPassoAvl::RotacaoEsquerda
                    : TipoEventoPassoAvl::RotacaoDireita,
                (*ligacao)->valor, antes, (*ligacao)->valor};
    }

    [[nodiscard]] EventoPassoAvl<Valor> concluirInsercao(
        std::optional<Valor> valor)
    {
        fase_ = Fase::Ociosa;
        valorPendente_.reset();
        ligacaoAtual_ = nullptr;
        return {TipoEventoPassoAvl::InsercaoConcluida, std::move(valor),
                std::nullopt, std::nullopt};
    }

    [[nodiscard]] static std::size_t alturaDoNo(
        const std::unique_ptr<No>& no) noexcept
    {
        return no ? no->altura : 0;
    }

    static void atualizarAltura(No& no) noexcept
    {
        no.altura = 1 + std::max(alturaDoNo(no.esquerdo), alturaDoNo(no.direito));
    }

    [[nodiscard]] static int fatorBalanceamento(const No& no) noexcept
    {
        return static_cast<int>(alturaDoNo(no.esquerdo))
            - static_cast<int>(alturaDoNo(no.direito));
    }

    [[nodiscard]] std::unique_ptr<No> rotacionarEsquerda(
        std::unique_ptr<No> no) noexcept
    {
        ++quantidadeRotacoes_;
        auto novaRaiz = std::move(no->direito);
        no->direito = std::move(novaRaiz->esquerdo);
        atualizarAltura(*no);
        novaRaiz->esquerdo = std::move(no);
        atualizarAltura(*novaRaiz);
        return novaRaiz;
    }

    [[nodiscard]] std::unique_ptr<No> rotacionarDireita(
        std::unique_ptr<No> no) noexcept
    {
        ++quantidadeRotacoes_;
        auto novaRaiz = std::move(no->esquerdo);
        no->esquerdo = std::move(novaRaiz->direito);
        atualizarAltura(*no);
        novaRaiz->direito = std::move(no);
        atualizarAltura(*novaRaiz);
        return novaRaiz;
    }

    template<typename Visitante>
    static void visitarNosEmOrdem(const No* no, const No* pai,
                                  bool filhoEsquerdo, std::size_t profundidade,
                                  Visitante& visitante)
    {
        if (!no) return;
        visitarNosEmOrdem(no->esquerdo.get(), no, true, profundidade + 1, visitante);
        visitante(no->valor, pai ? &pai->valor : nullptr,
                  filhoEsquerdo, profundidade);
        visitarNosEmOrdem(no->direito.get(), no, false, profundidade + 1, visitante);
    }

    std::unique_ptr<No> raiz_;
    std::size_t quantidade_ = 0;
    std::size_t quantidadeRotacoes_ = 0;
    [[no_unique_address]] Comparador comparador_;
    std::optional<Valor> valorPendente_;
    Ligacao ligacaoAtual_ = nullptr;
    std::vector<Ligacao> caminho_;
    Ligacao ligacaoRotacaoPendente_ = nullptr;
    Rotacao rotacaoPendente_ = Rotacao::Esquerda;
    Fase fase_ = Fase::Ociosa;
};

} // namespace estruturas
