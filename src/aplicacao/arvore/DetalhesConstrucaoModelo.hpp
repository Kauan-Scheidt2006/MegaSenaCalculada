#pragma once

#include "dominio/Registro.hpp"
#include "interface/apresentacao/arvore/ModeloVisualArvore.hpp"

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>

namespace aplicacao::arvore::detalhes {

template<typename Arvore>
bool metricasPeriodicasCalculadas(const Arvore& arvore)
{
    if constexpr (requires { arvore.metricasPeriodicasCalculadas(); }) {
        return arvore.metricasPeriodicasCalculadas();
    }
    return false;
}

template<typename Arvore>
std::vector<megasena::AmostraAlturaArvore> obterMetricasPeriodicas(
    const Arvore& arvore)
{
    if constexpr (requires { arvore.metricasPeriodicas(); }) {
        return arvore.metricasPeriodicas();
    }
    return {};
}

struct ComparadorRegistroContabilizado {
    std::shared_ptr<std::size_t> quantidade;

    [[nodiscard]] bool operator()(const megasena::Registro& esquerda,
                                  const megasena::Registro& direita) const noexcept
    {
        ++*quantidade;
        return megasena::ComparadorRegistro{}(esquerda, direita);
    }
};

template<typename Arvore, typename VisitarNos>
apresentacao::arvore::ModeloVisualArvore criarModeloVisualComSemantica(
    const Arvore& arvore, std::size_t comparacoes, std::size_t rotacoes,
    VisitarNos visitarNos, std::uint64_t tempoInsercaoNanossegundos = 0)
{
    std::vector<apresentacao::arvore::ModeloNoArvore> nos;
    std::vector<apresentacao::arvore::ModeloLigacaoArvore> ligacoes;
    std::optional<apresentacao::arvore::IdentificadorRegistro> raiz;
    nos.reserve(arvore.quantidade());
    if (!arvore.vazia()) {
        ligacoes.reserve(arvore.quantidade() - 1);
    }

    visitarNos(
        [&](const megasena::Registro& registro, const megasena::Registro* pai,
            bool filhoEsquerdo, std::size_t profundidade,
            apresentacao::arvore::SemanticaVisualNo semanticaVisual) {
            (void)profundidade;
            const apresentacao::arvore::IdentificadorRegistro identificador{registro.chave.frequenciaAcumulada, registro.chave.numero};
            nos.push_back({identificador, semanticaVisual});
            if (!pai) {
                raiz = identificador;
                return;
            }

            const apresentacao::arvore::IdentificadorRegistro identificadorPai{
                pai->chave.frequenciaAcumulada, pai->chave.numero};
            ligacoes.push_back(
                {identificadorPai, identificador,
                 filhoEsquerdo
                     ? apresentacao::arvore::LadoLigacaoArvore::Esquerdo
                     : apresentacao::arvore::LadoLigacaoArvore::Direito});
        });

    return {raiz, std::move(nos), std::move(ligacoes),
            {arvore.quantidade(), arvore.altura(), comparacoes, rotacoes,
             tempoInsercaoNanossegundos,
             metricasPeriodicasCalculadas(arvore),
             obterMetricasPeriodicas(arvore)}};
}

template<typename Arvore>
apresentacao::arvore::ModeloVisualArvore criarModeloVisual(
    const Arvore& arvore, std::size_t comparacoes, std::size_t rotacoes,
    std::uint64_t tempoInsercaoNanossegundos = 0)
{
    return criarModeloVisualComSemantica(
        arvore, comparacoes, rotacoes,
        [&arvore](auto visitante) {
            arvore.visitarNosEmOrdem(
                [&visitante](const megasena::Registro& registro,
                             const megasena::Registro* pai,
                             bool filhoEsquerdo, std::size_t profundidade) {
                    visitante(
                        registro, pai, filhoEsquerdo, profundidade,
                        apresentacao::arvore::SemanticaVisualNo::Padrao);
                });
        },
        tempoInsercaoNanossegundos);
}

template<typename Arvore>
struct ResultadoConstrucaoArvore {
    Arvore arvore;
    std::uint64_t tempoInsercaoNanossegundos = 0;
    std::size_t comparacoes = 0;
};

template<typename Arvore>
ResultadoConstrucaoArvore<Arvore> construirArvore(
    const std::vector<megasena::Registro>& registros,
    const std::shared_ptr<std::size_t>& comparacoes)
{
    Arvore arvore(ComparadorRegistroContabilizado{comparacoes});
    const auto inicio = std::chrono::steady_clock::now();
    for (const auto& registro : registros) {
        if (!arvore.inserir(registro)) {
            throw std::invalid_argument(
                "A sequencia historica produziu uma chave duplicada.");
        }
    }
    const auto fim = std::chrono::steady_clock::now();
    const auto duracao = std::chrono::duration_cast<std::chrono::nanoseconds>(
        fim - inicio);
    return {std::move(arvore), static_cast<std::uint64_t>(duracao.count()),
            *comparacoes};
}

} // namespace aplicacao::arvore::detalhes
