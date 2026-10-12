#pragma once

#include "estruturas/NoRubroNegro.hpp"
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
class ArvoreRubroNegra {
private:
    using No = NoRubroNegro<Valor>;

    struct ResultadoValidacao {
        bool valido;
        std::size_t alturaPreta;
    };

public:
    ArvoreRubroNegra() = default;

    explicit ArvoreRubroNegra(Comparador comparador)
        : comparador_(std::move(comparador))
    {
    }

    ArvoreRubroNegra(const ArvoreRubroNegra&) = delete;
    ArvoreRubroNegra& operator=(const ArvoreRubroNegra&) = delete;
    ArvoreRubroNegra(ArvoreRubroNegra&&) noexcept = default;
    ArvoreRubroNegra& operator=(ArvoreRubroNegra&&) noexcept = default;
    ~ArvoreRubroNegra() = default;

    [[nodiscard]] bool inserir(const Valor& valor)
    {
        No* inserido = inserirNaSubarvore(raiz_, nullptr, valor);
        if (!inserido) {
            return false;
        }

        corrigirInsercao(inserido);
        raiz_->cor = CorNoRubroNegro::Preto;
        raiz_->pai = nullptr;
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
        No* removido = encontrarNo(valor, metricas);
        if (!removido) {
            return false;
        }

        removerNaSubarvore(removido, metricas);
        --quantidade_;
        if (raiz_) {
            raiz_->cor = CorNoRubroNegro::Preto;
            raiz_->pai = nullptr;
        }
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

    template<typename Visitante>
    void visitarNosComCorEmOrdem(Visitante visitante) const
    {
        visitarNosComCorEmOrdem(raiz_.get(), nullptr, false, 0, visitante);
    }

    [[nodiscard]] bool invariantesPreservadas() const
    {
        if (!raiz_) {
            return quantidade_ == 0;
        }
        if (raiz_->cor != CorNoRubroNegro::Preto) {
            return false;
        }

        const ResultadoValidacao resultado =
            validarSubarvore(raiz_.get(), nullptr, nullptr, nullptr);
        return resultado.valido;
    }

private:
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

    [[nodiscard]] static bool vermelho(const No* no) noexcept
    {
        return no && no->cor == CorNoRubroNegro::Vermelho;
    }

    [[nodiscard]] std::unique_ptr<No> rotacionarDireita(
        std::unique_ptr<No> raizSubarvore) noexcept
    {
        ++quantidadeRotacoes_;
        No* pai = raizSubarvore->pai;
        std::unique_ptr<No> novaRaiz = std::move(raizSubarvore->esquerdo);
        raizSubarvore->esquerdo = std::move(novaRaiz->direito);
        if (raizSubarvore->esquerdo) {
            raizSubarvore->esquerdo->pai = raizSubarvore.get();
        }
        novaRaiz->direito = std::move(raizSubarvore);
        novaRaiz->direito->pai = novaRaiz.get();
        novaRaiz->pai = pai;
        return novaRaiz;
    }

    [[nodiscard]] std::unique_ptr<No> rotacionarEsquerda(
        std::unique_ptr<No> raizSubarvore) noexcept
    {
        ++quantidadeRotacoes_;
        No* pai = raizSubarvore->pai;
        std::unique_ptr<No> novaRaiz = std::move(raizSubarvore->direito);
        raizSubarvore->direito = std::move(novaRaiz->esquerdo);
        if (raizSubarvore->direito) {
            raizSubarvore->direito->pai = raizSubarvore.get();
        }
        novaRaiz->esquerdo = std::move(raizSubarvore);
        novaRaiz->esquerdo->pai = novaRaiz.get();
        novaRaiz->pai = pai;
        return novaRaiz;
    }

    void corrigirInsercao(No* no) noexcept
    {
        while (no != raiz_.get() && vermelho(no->pai)) {
            No* pai = no->pai;
            No* avo = pai->pai;

            if (pai == avo->esquerdo.get()) {
                No* tio = avo->direito.get();
                if (vermelho(tio)) {
                    pai->cor = CorNoRubroNegro::Preto;
                    tio->cor = CorNoRubroNegro::Preto;
                    avo->cor = CorNoRubroNegro::Vermelho;
                    no = avo;
                    continue;
                }

                if (no == pai->direito.get()) {
                    no = pai;
                    auto& ligacaoPai = ligacaoDoNo(no);
                    ligacaoPai = rotacionarEsquerda(std::move(ligacaoPai));
                    pai = no->pai;
                    avo = pai->pai;
                }

                pai->cor = CorNoRubroNegro::Preto;
                avo->cor = CorNoRubroNegro::Vermelho;
                auto& ligacaoAvo = ligacaoDoNo(avo);
                ligacaoAvo = rotacionarDireita(std::move(ligacaoAvo));
            } else {
                No* tio = avo->esquerdo.get();
                if (vermelho(tio)) {
                    pai->cor = CorNoRubroNegro::Preto;
                    tio->cor = CorNoRubroNegro::Preto;
                    avo->cor = CorNoRubroNegro::Vermelho;
                    no = avo;
                    continue;
                }

                if (no == pai->esquerdo.get()) {
                    no = pai;
                    auto& ligacaoPai = ligacaoDoNo(no);
                    ligacaoPai = rotacionarDireita(std::move(ligacaoPai));
                    pai = no->pai;
                    avo = pai->pai;
                }

                pai->cor = CorNoRubroNegro::Preto;
                avo->cor = CorNoRubroNegro::Vermelho;
                auto& ligacaoAvo = ligacaoDoNo(avo);
                ligacaoAvo = rotacionarEsquerda(std::move(ligacaoAvo));
            }
        }

        raiz_->cor = CorNoRubroNegro::Preto;
        raiz_->pai = nullptr;
    }

    [[nodiscard]] No* inserirNaSubarvore(
        std::unique_ptr<No>& no, No* pai, const Valor& valor)
    {
        if (!no) {
            no = std::make_unique<No>(valor);
            no->pai = pai;
            return no.get();
        }

        if (comparador_(valor, no->valor)) {
            return inserirNaSubarvore(no->esquerdo, no.get(), valor);
        } else if (comparador_(no->valor, valor)) {
            return inserirNaSubarvore(no->direito, no.get(), valor);
        }
        return nullptr;
    }

    [[nodiscard]] No* encontrarNo(
        const Valor& valor, MetricasNavegacaoArvore& metricas) noexcept
    {
        No* atual = raiz_.get();
        while (atual) {
            ++metricas.nosVisitados;
            ++metricas.comparacoes;
            if (comparador_(valor, atual->valor)) {
                atual = atual->esquerdo.get();
            } else {
                ++metricas.comparacoes;
                if (comparador_(atual->valor, valor)) {
                    atual = atual->direito.get();
                } else {
                    return atual;
                }
            }
        }
        return nullptr;
    }

    [[nodiscard]] std::unique_ptr<No>& ligacaoDoNo(No* no) noexcept
    {
        if (!no->pai) {
            return raiz_;
        }
        if (no->pai->esquerdo.get() == no) {
            return no->pai->esquerdo;
        }
        return no->pai->direito;
    }

    [[nodiscard]] static No* minimo(
        No* no, MetricasNavegacaoArvore& metricas) noexcept
    {
        ++metricas.nosVisitados;
        while (no->esquerdo) {
            no = no->esquerdo.get();
            ++metricas.nosVisitados;
        }
        return no;
    }

    [[nodiscard]] static bool preto(const No* no) noexcept
    {
        return !no || no->cor == CorNoRubroNegro::Preto;
    }

    void removerNaSubarvore(
        No* alvo, MetricasNavegacaoArvore& metricas)
    {
        No* substitutoCor = alvo;
        CorNoRubroNegro corRemovida = substitutoCor->cor;
        No* filhoDeficit = nullptr;
        No* paiDeficit = nullptr;

        auto& ligacaoAlvo = ligacaoDoNo(alvo);
        if (!alvo->esquerdo || !alvo->direito) {
            auto antigo = std::move(ligacaoAlvo);
            auto substituto = antigo->esquerdo
                ? std::move(antigo->esquerdo)
                : std::move(antigo->direito);
            paiDeficit = antigo->pai;
            filhoDeficit = substituto.get();
            if (substituto) {
                substituto->pai = paiDeficit;
            }
            ligacaoAlvo = std::move(substituto);
        } else {
            substitutoCor = minimo(alvo->direito.get(), metricas);
            corRemovida = substitutoCor->cor;
            No* paiOriginalSucessor = substitutoCor->pai;
            const bool sucessorDireto = paiOriginalSucessor == alvo;

            auto& ligacaoSucessor = ligacaoDoNo(substitutoCor);
            auto sucessor = std::move(ligacaoSucessor);
            auto filhoSucessor = std::move(sucessor->direito);
            filhoDeficit = filhoSucessor.get();
            if (filhoSucessor) {
                filhoSucessor->pai = paiOriginalSucessor;
            }
            ligacaoSucessor = std::move(filhoSucessor);

            auto antigo = std::move(ligacaoAlvo);
            sucessor->pai = antigo->pai;
            sucessor->cor = antigo->cor;
            sucessor->esquerdo = std::move(antigo->esquerdo);
            sucessor->esquerdo->pai = sucessor.get();
            sucessor->direito = std::move(antigo->direito);
            if (sucessor->direito) {
                sucessor->direito->pai = sucessor.get();
            }
            ligacaoAlvo = std::move(sucessor);

            paiDeficit = sucessorDireto
                ? ligacaoAlvo.get()
                : paiOriginalSucessor;
        }

        if (corRemovida == CorNoRubroNegro::Preto) {
            corrigirDeficitPreto(filhoDeficit, paiDeficit);
        }
    }

    void corrigirDeficitPreto(No* no, No* pai)
    {
        while (no != raiz_.get() && preto(no)) {
            if (!pai) {
                break;
            }

            if (pai->esquerdo.get() == no) {
                No* irmao = pai->direito.get();
                if (!preto(irmao)) {
                    irmao->cor = CorNoRubroNegro::Preto;
                    pai->cor = CorNoRubroNegro::Vermelho;
                    auto& ligacaoPai = ligacaoDoNo(pai);
                    ligacaoPai = rotacionarEsquerda(std::move(ligacaoPai));
                    irmao = pai->direito.get();
                }

                if (!irmao) {
                    no = pai;
                    pai = no->pai;
                    continue;
                }
                if (preto(irmao->esquerdo.get())
                    && preto(irmao->direito.get())) {
                    irmao->cor = CorNoRubroNegro::Vermelho;
                    no = pai;
                    pai = no->pai;
                } else {
                    if (preto(irmao->direito.get())) {
                        if (irmao->esquerdo) {
                            irmao->esquerdo->cor = CorNoRubroNegro::Preto;
                        }
                        irmao->cor = CorNoRubroNegro::Vermelho;
                        auto& ligacaoIrmao = ligacaoDoNo(irmao);
                        ligacaoIrmao = rotacionarDireita(
                            std::move(ligacaoIrmao));
                        irmao = pai->direito.get();
                    }
                    irmao->cor = pai->cor;
                    pai->cor = CorNoRubroNegro::Preto;
                    if (irmao->direito) {
                        irmao->direito->cor = CorNoRubroNegro::Preto;
                    }
                    auto& ligacaoPai = ligacaoDoNo(pai);
                    ligacaoPai = rotacionarEsquerda(std::move(ligacaoPai));
                    no = raiz_.get();
                    pai = nullptr;
                }
            } else {
                No* irmao = pai->esquerdo.get();
                if (!preto(irmao)) {
                    irmao->cor = CorNoRubroNegro::Preto;
                    pai->cor = CorNoRubroNegro::Vermelho;
                    auto& ligacaoPai = ligacaoDoNo(pai);
                    ligacaoPai = rotacionarDireita(std::move(ligacaoPai));
                    irmao = pai->esquerdo.get();
                }

                if (!irmao) {
                    no = pai;
                    pai = no->pai;
                    continue;
                }
                if (preto(irmao->direito.get())
                    && preto(irmao->esquerdo.get())) {
                    irmao->cor = CorNoRubroNegro::Vermelho;
                    no = pai;
                    pai = no->pai;
                } else {
                    if (preto(irmao->esquerdo.get())) {
                        if (irmao->direito) {
                            irmao->direito->cor = CorNoRubroNegro::Preto;
                        }
                        irmao->cor = CorNoRubroNegro::Vermelho;
                        auto& ligacaoIrmao = ligacaoDoNo(irmao);
                        ligacaoIrmao = rotacionarEsquerda(
                            std::move(ligacaoIrmao));
                        irmao = pai->esquerdo.get();
                    }
                    irmao->cor = pai->cor;
                    pai->cor = CorNoRubroNegro::Preto;
                    if (irmao->esquerdo) {
                        irmao->esquerdo->cor = CorNoRubroNegro::Preto;
                    }
                    auto& ligacaoPai = ligacaoDoNo(pai);
                    ligacaoPai = rotacionarDireita(std::move(ligacaoPai));
                    no = raiz_.get();
                    pai = nullptr;
                }
            }
        }

        if (no) {
            no->cor = CorNoRubroNegro::Preto;
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

    template<typename Visitante>
    static void visitarNosComCorEmOrdem(const No* no, const No* pai,
                                        bool filhoEsquerdo,
                                        std::size_t profundidade,
                                        Visitante& visitante)
    {
        if (!no) {
            return;
        }

        visitarNosComCorEmOrdem(no->esquerdo.get(), no, true,
                                profundidade + 1, visitante);
        visitante(no->valor, pai ? &pai->valor : nullptr,
                  filhoEsquerdo, profundidade, no->cor);
        visitarNosComCorEmOrdem(no->direito.get(), no, false,
                                profundidade + 1, visitante);
    }

    [[nodiscard]] ResultadoValidacao validarSubarvore(
        const No* no, const No* paiEsperado,
        const Valor* limiteInferior, const Valor* limiteSuperior) const
    {
        if (!no) {
            return {true, 1};
        }
        if (no->pai != paiEsperado) {
            return {false, 0};
        }
        if ((limiteInferior && !comparador_(*limiteInferior, no->valor))
            || (limiteSuperior && !comparador_(no->valor, *limiteSuperior))) {
            return {false, 0};
        }
        if (no->cor == CorNoRubroNegro::Vermelho
            && (vermelho(no->esquerdo.get())
                || vermelho(no->direito.get()))) {
            return {false, 0};
        }

        const ResultadoValidacao esquerda =
            validarSubarvore(
                no->esquerdo.get(), no, limiteInferior, &no->valor);
        const ResultadoValidacao direita =
            validarSubarvore(
                no->direito.get(), no, &no->valor, limiteSuperior);
        if (!esquerda.valido || !direita.valido
            || esquerda.alturaPreta != direita.alturaPreta) {
            return {false, 0};
        }

        return {true, esquerda.alturaPreta
                          + (no->cor == CorNoRubroNegro::Preto ? 1U : 0U)};
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
