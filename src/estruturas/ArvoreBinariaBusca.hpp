#pragma once

#include "dominio/AmostraAlturaArvore.hpp"
#include "estruturas/MetricasNavegacaoArvore.hpp"

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
        //calcularMetricasPeriodicas();
        return true;
    }

    [[nodiscard]] bool remover(const Valor& valor)
    {
        MetricasNavegacaoArvore metricas;
        return remover(valor, metricas);
    }

    [[nodiscard]] bool remover(
        const Valor& valor, MetricasNavegacaoArvore& metricas)
    {
        metricas = {};
        if (!removerNaSubarvore(raiz_, valor, metricas)) {
            return false;
        }
        --quantidade_;
        return true;
    }

    [[nodiscard]] const Valor* buscar(const Valor& valor) const noexcept
    {
        MetricasNavegacaoArvore metricas;
        return buscarNaSubarvore(raiz_.get(), valor, metricas);
    }

    [[nodiscard]] const Valor* buscar(
        const Valor& valor, MetricasNavegacaoArvore& metricas) const noexcept
    {
        metricas = {};
        return buscarNaSubarvore(raiz_.get(), valor, metricas);
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

    [[nodiscard]] const std::vector<megasena::AmostraAlturaArvore>&
    metricasPeriodicas() const noexcept
    {
        return metricasPeriodicas_;
    }

    [[nodiscard]] bool metricasPeriodicasCalculadas() const noexcept
    {
        return metricasPeriodicasCalculadas_;
    }

    [[nodiscard]] std::vector<Valor> percorrerEmOrdem() const
    {
        std::vector<Valor> valores;
        valores.reserve(quantidade_);
        adicionarEmOrdem(raiz_.get(), valores);
        return valores;
    }

    [[nodiscard]] std::vector<Valor> percorrerPreOrdem() const
    {
        std::vector<Valor> valores;
        valores.reserve(quantidade_);
        adicionarPreOrdem(raiz_.get(), valores);
        return valores;
    }

    [[nodiscard]] std::vector<Valor> percorrerPosOrdem() const
    {
        std::vector<Valor> valores;
        valores.reserve(quantidade_);
        adicionarPosOrdem(raiz_.get(), valores);
        return valores;
    }

    template<typename Visitante>
    void visitarNosEmOrdem(Visitante visitante) const
    {
        visitarNosEmOrdem(raiz_.get(), nullptr, false, 0, visitante);
    }

private:
    [[nodiscard]] bool removerNaSubarvore(
        std::unique_ptr<No>& no, const Valor& valor,
        MetricasNavegacaoArvore& metricas)
    {
        if (!no) {
            return false;
        }

        ++metricas.nosVisitados;
        ++metricas.comparacoes;
        if (comparador_(valor, no->valor)) {
            return removerNaSubarvore(no->esquerdo, valor, metricas);
        }
        ++metricas.comparacoes;
        if (comparador_(no->valor, valor)) {
            return removerNaSubarvore(no->direito, valor, metricas);
        }

        if (!no->esquerdo) {
            no = std::move(no->direito);
            return true;
        }
        if (!no->direito) {
            no = std::move(no->esquerdo);
            return true;
        }

        auto removido = std::move(no);
        auto sucessor = extrairMinimo(removido->direito, metricas);
        sucessor->esquerdo = std::move(removido->esquerdo);
        sucessor->direito = std::move(removido->direito);
        no = std::move(sucessor);
        return true;
    }

    [[nodiscard]] static std::unique_ptr<No> extrairMinimo(
        std::unique_ptr<No>& subarvore,
        MetricasNavegacaoArvore& metricas)
    {
        ++metricas.nosVisitados;
        if (subarvore->esquerdo) {
            return extrairMinimo(subarvore->esquerdo, metricas);
        }

        auto minimo = std::move(subarvore);
        subarvore = std::move(minimo->direito);
        return minimo;
    }

    void calcularMetricasPeriodicas()
    {
        metricasPeriodicasCalculadas_ = true;
        if (quantidade_ % IntervaloMetricasPeriodicas == 0) {
            metricasPeriodicas_.push_back({quantidade_, altura()});
        }
    }

    [[nodiscard]] const Valor* buscarNaSubarvore(
        const No* no, const Valor& valor,
        MetricasNavegacaoArvore& metricas) const noexcept
    {
        if (!no) {
            return nullptr;
        }
        ++metricas.nosVisitados;
        ++metricas.comparacoes;
        if (comparador_(valor, no->valor)) {
            return buscarNaSubarvore(
                no->esquerdo.get(), valor, metricas);
        }
        ++metricas.comparacoes;
        if (comparador_(no->valor, valor)) {
            return buscarNaSubarvore(
                no->direito.get(), valor, metricas);
        }
        return &no->valor;
    }

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

    static void adicionarPreOrdem(const No* no, std::vector<Valor>& valores)
    {
        if (!no) {
            return;
        }

        valores.push_back(no->valor);
        adicionarPreOrdem(no->esquerdo.get(), valores);
        adicionarPreOrdem(no->direito.get(), valores);
    }

    static void adicionarPosOrdem(const No* no, std::vector<Valor>& valores)
    {
        if (!no) {
            return;
        }

        adicionarPosOrdem(no->esquerdo.get(), valores);
        adicionarPosOrdem(no->direito.get(), valores);
        valores.push_back(no->valor);
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
    static constexpr std::size_t IntervaloMetricasPeriodicas = 600;
    std::vector<megasena::AmostraAlturaArvore> metricasPeriodicas_;
    bool metricasPeriodicasCalculadas_ = false;
    [[no_unique_address]] Comparador comparador_;
};

} // namespace estruturas
