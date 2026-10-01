#pragma once

#include "estruturas/NoAvl.hpp"

#include <algorithm>
#include <cstddef>
#include <functional>
#include <memory>
#include <utility>
#include <vector>

namespace estruturas {

template<typename Valor, typename Comparador = std::less<Valor>>
class ArvoreAvl {
private:
    using No = NoAvl<Valor>;

public:
    ArvoreAvl() = default;

    explicit ArvoreAvl(Comparador comparador)
        : comparador_(std::move(comparador))
    {
    }

    ArvoreAvl(const ArvoreAvl&) = delete;
    ArvoreAvl& operator=(const ArvoreAvl&) = delete;
    ArvoreAvl(ArvoreAvl&&) noexcept = default;
    ArvoreAvl& operator=(ArvoreAvl&&) noexcept = default;
    ~ArvoreAvl() = default;

    [[nodiscard]] bool inserir(const Valor& valor)
    {
        bool inserido = false;
        inserirNaSubarvore(raiz_, valor, inserido);

        if (inserido) {
            ++quantidade_;
        }

        return inserido;
    }

    [[nodiscard]] bool vazia() const noexcept
    {
        return raiz_ == nullptr;
    }

    [[nodiscard]] std::size_t quantidade() const noexcept
    {
        return quantidade_;
    }

    [[nodiscard]] std::size_t altura() const noexcept
    {
        return alturaDoNo(raiz_);
    }

    [[nodiscard]] std::vector<Valor> percorrerEmOrdem() const
    {
        std::vector<Valor> valores;
        valores.reserve(quantidade_);
        adicionarEmOrdem(raiz_.get(), valores);
        return valores;
    }

private:
    [[nodiscard]] static std::size_t alturaDoNo(
        const std::unique_ptr<No>& no) noexcept
    {
        return no ? no->altura : 0;
    }

    static void atualizarAltura(No& no) noexcept
    {
        no.altura = 1 + std::max(alturaDoNo(no.esquerdo),
                                 alturaDoNo(no.direito));
    }

    [[nodiscard]] static int calcularFatorBalanceamento(
        const No& no) noexcept
    {
        const int alturaEsquerda =
            static_cast<int>(alturaDoNo(no.esquerdo));
        const int alturaDireita =
            static_cast<int>(alturaDoNo(no.direito));
        return alturaEsquerda - alturaDireita;
    }

    [[nodiscard]] static std::unique_ptr<No> rotacionarDireita(
        std::unique_ptr<No> raizSubarvore) noexcept
    {
        std::unique_ptr<No> novaRaiz = std::move(raizSubarvore->esquerdo);
        raizSubarvore->esquerdo = std::move(novaRaiz->direito);
        atualizarAltura(*raizSubarvore);

        novaRaiz->direito = std::move(raizSubarvore);
        atualizarAltura(*novaRaiz);
        return novaRaiz;
    }

    [[nodiscard]] static std::unique_ptr<No> rotacionarEsquerda(
        std::unique_ptr<No> raizSubarvore) noexcept
    {
        std::unique_ptr<No> novaRaiz = std::move(raizSubarvore->direito);
        raizSubarvore->direito = std::move(novaRaiz->esquerdo);
        atualizarAltura(*raizSubarvore);

        novaRaiz->esquerdo = std::move(raizSubarvore);
        atualizarAltura(*novaRaiz);
        return novaRaiz;
    }

    [[nodiscard]] static std::unique_ptr<No> rebalancear(
        std::unique_ptr<No> no) noexcept
    {
        atualizarAltura(*no);
        const int fator = calcularFatorBalanceamento(*no);

        if (fator > 1) {
            if (calcularFatorBalanceamento(*no->esquerdo) < 0) {
                no->esquerdo = rotacionarEsquerda(std::move(no->esquerdo));
            }
            return rotacionarDireita(std::move(no));
        }

        if (fator < -1) {
            if (calcularFatorBalanceamento(*no->direito) > 0) {
                no->direito = rotacionarDireita(std::move(no->direito));
            }
            return rotacionarEsquerda(std::move(no));
        }

        return no;
    }

    void inserirNaSubarvore(std::unique_ptr<No>& no, const Valor& valor,
                            bool& inserido)
    {
        if (!no) {
            no = std::make_unique<No>(valor);
            inserido = true;
            return;
        }

        if (comparador_(valor, no->valor)) {
            inserirNaSubarvore(no->esquerdo, valor, inserido);
        } else if (comparador_(no->valor, valor)) {
            inserirNaSubarvore(no->direito, valor, inserido);
        } else {
            return;
        }

        if (inserido) {
            no = rebalancear(std::move(no));
        }
    }

    static void adicionarEmOrdem(const No* no, std::vector<Valor>& valores)
    {
        if (!no) {
            return;
        }

        adicionarEmOrdem(no->esquerdo.get(), valores);
        valores.push_back(no->valor);
        adicionarEmOrdem(no->direito.get(), valores);
    }

    std::unique_ptr<No> raiz_;
    std::size_t quantidade_ = 0;
    [[no_unique_address]] Comparador comparador_;
};

} // namespace estruturas
