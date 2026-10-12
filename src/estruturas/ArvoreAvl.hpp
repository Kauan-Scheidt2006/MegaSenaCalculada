#pragma once

#include "estruturas/NoAvl.hpp"
#include "estruturas/MetricasNavegacaoArvore.hpp"
#include "dominio/AmostraAlturaArvore.hpp"

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
            //calcularMetricasPeriodicas();
        }

        return inserido;
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

    [[nodiscard]] const std::vector<megasena::AmostraAlturaArvore>&
    metricasPeriodicas() const noexcept
    {
        return metricasPeriodicas_;
    }

    [[nodiscard]] bool metricasPeriodicasCalculadas() const noexcept
    {
        return metricasPeriodicasCalculadas_;
    }

    [[nodiscard]] std::size_t quantidadeRotacoes() const noexcept
    {
        return quantidadeRotacoes_;
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
        bool removido = false;
        ++metricas.comparacoes;
        if (comparador_(valor, no->valor)) {
            removido = removerNaSubarvore(
                no->esquerdo, valor, metricas);
        } else {
            ++metricas.comparacoes;
            if (comparador_(no->valor, valor)) {
                removido = removerNaSubarvore(
                    no->direito, valor, metricas);
            } else if (!no->esquerdo) {
                no = std::move(no->direito);
                return true;
            } else if (!no->direito) {
                no = std::move(no->esquerdo);
                return true;
            } else {
                auto antigo = std::move(no);
                auto sucessor = extrairMinimo(antigo->direito, metricas);
                sucessor->esquerdo = std::move(antigo->esquerdo);
                sucessor->direito = std::move(antigo->direito);
                no = rebalancear(std::move(sucessor));
                return true;
            }
        }

        if (removido && no) {
            no = rebalancear(std::move(no));
        }
        return removido;
    }

    [[nodiscard]] std::unique_ptr<No> extrairMinimo(
        std::unique_ptr<No>& subarvore,
        MetricasNavegacaoArvore& metricas)
    {
        ++metricas.nosVisitados;
        if (!subarvore->esquerdo) {
            auto minimo = std::move(subarvore);
            subarvore = std::move(minimo->direito);
            return minimo;
        }

        auto minimo = extrairMinimo(subarvore->esquerdo, metricas);
        subarvore = rebalancear(std::move(subarvore));
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

    [[nodiscard]] std::unique_ptr<No> rotacionarDireita(
        std::unique_ptr<No> raizSubarvore) noexcept
    {
        ++quantidadeRotacoes_;
        std::unique_ptr<No> novaRaiz = std::move(raizSubarvore->esquerdo);
        raizSubarvore->esquerdo = std::move(novaRaiz->direito);
        atualizarAltura(*raizSubarvore);

        novaRaiz->direito = std::move(raizSubarvore);
        atualizarAltura(*novaRaiz);
        return novaRaiz;
    }

    [[nodiscard]] std::unique_ptr<No> rotacionarEsquerda(
        std::unique_ptr<No> raizSubarvore) noexcept
    {
        ++quantidadeRotacoes_;
        std::unique_ptr<No> novaRaiz = std::move(raizSubarvore->direito);
        raizSubarvore->direito = std::move(novaRaiz->esquerdo);
        atualizarAltura(*raizSubarvore);

        novaRaiz->esquerdo = std::move(raizSubarvore);
        atualizarAltura(*novaRaiz);
        return novaRaiz;
    }

    [[nodiscard]] std::unique_ptr<No> rebalancear(std::unique_ptr<No> no) noexcept
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
                                  bool filhoEsquerdo,
                                  std::size_t profundidade,
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
    std::size_t quantidadeRotacoes_ = 0;
    static constexpr std::size_t IntervaloMetricasPeriodicas = 600;
    std::vector<megasena::AmostraAlturaArvore> metricasPeriodicas_;
    bool metricasPeriodicasCalculadas_ = false;
    [[no_unique_address]] Comparador comparador_;
};

} // namespace estruturas
