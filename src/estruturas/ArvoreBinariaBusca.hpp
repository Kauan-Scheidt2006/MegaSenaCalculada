#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <memory>
#include <utility>
#include <vector>

namespace estruturas {

template<typename Valor, typename Comparador = std::less<Valor>>
class ArvoreBinariaBusca {
private:
    struct No {
        explicit No(const Valor& novoValor)
            : valor(novoValor)
        {
        }

        Valor valor;
        std::unique_ptr<No> esquerdo;
        std::unique_ptr<No> direito;
    };

public:
    ArvoreBinariaBusca() = default;

    explicit ArvoreBinariaBusca(Comparador comparador)
        : comparador_(std::move(comparador))
    {
    }

    ArvoreBinariaBusca(const ArvoreBinariaBusca&) = delete;
    ArvoreBinariaBusca& operator=(const ArvoreBinariaBusca&) = delete;
    ArvoreBinariaBusca(ArvoreBinariaBusca&&) noexcept = default;
    ArvoreBinariaBusca& operator=(ArvoreBinariaBusca&&) noexcept = default;
    ~ArvoreBinariaBusca() = default;

    [[nodiscard]] bool inserir(const Valor& valor)
    {
        std::unique_ptr<No>* posicao = &raiz_;

        while (*posicao) {
            if (comparador_(valor, (*posicao)->valor)) {
                posicao = &(*posicao)->esquerdo;
            } else if (comparador_((*posicao)->valor, valor)) {
                posicao = &(*posicao)->direito;
            } else {
                return false;
            }
        }

        *posicao = std::make_unique<No>(valor);
        ++quantidade_;
        return true;
    }

    [[nodiscard]] const Valor* buscar(const Valor& valor) const noexcept
    {
        const No* atual = raiz_.get();

        while (atual) {
            if (comparador_(valor, atual->valor)) {
                atual = atual->esquerdo.get();
            } else if (comparador_(atual->valor, valor)) {
                atual = atual->direito.get();
            } else {
                return &atual->valor;
            }
        }

        return nullptr;
    }

    [[nodiscard]] bool contem(const Valor& valor) const noexcept
    {
        return buscar(valor) != nullptr;
    }

    [[nodiscard]] bool vazia() const noexcept
    {
        return quantidade_ == 0;
    }

    [[nodiscard]] std::size_t quantidade() const noexcept
    {
        return quantidade_;
    }

    [[nodiscard]] std::size_t altura() const noexcept
    {
        return calcularAltura(raiz_.get());
    }

    [[nodiscard]] std::vector<Valor> percorrerEmOrdem() const
    {
        std::vector<Valor> valores;
        valores.reserve(quantidade_);
        adicionarEmOrdem(raiz_.get(), valores);
        return valores;
    }

    template<typename Visitante>
    void visitarNosEmOrdem(Visitante visitante) const
    {
        visitarNosEmOrdem(raiz_.get(), nullptr, false, 0, visitante);
    }

private:
    [[nodiscard]] static std::size_t calcularAltura(const No* no) noexcept
    {
        if (!no) {
            return 0;
        }

        return 1 + std::max(calcularAltura(no->esquerdo.get()),
                            calcularAltura(no->direito.get()));
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

    template<typename Visitante>
    static void visitarNosEmOrdem(const No* no, const No* pai,
                                  bool filhoEsquerdo, std::size_t profundidade,
                                  Visitante& visitante)
    {
        if (!no) {
            return;
        }

        visitarNosEmOrdem(no->esquerdo.get(), no, true,
                          profundidade + 1, visitante);
        visitante(no->valor, pai ? &pai->valor : nullptr,
                  filhoEsquerdo, profundidade);
        visitarNosEmOrdem(no->direito.get(), no, false,
                          profundidade + 1, visitante);
    }

    std::unique_ptr<No> raiz_;
    std::size_t quantidade_ = 0;
    [[no_unique_address]] Comparador comparador_;
};

} // namespace estruturas
