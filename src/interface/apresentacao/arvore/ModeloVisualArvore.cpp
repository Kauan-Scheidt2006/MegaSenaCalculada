#include "interface/apresentacao/arvore/ModeloVisualArvore.hpp"

#include <cstddef>
#include <map>
#include <queue>
#include <stdexcept>
#include <utility>

namespace apresentacao::arvore {
namespace {

using IndicesPorIdentificador = std::map<IdentificadorRegistro, std::size_t>;

IndicesPorIdentificador indexarNos(const std::vector<ModeloNoArvore>& nos)
{
    IndicesPorIdentificador indices;
    for (std::size_t indice = 0; indice < nos.size(); ++indice) {
        if (!indices.emplace(nos[indice].identificador, indice).second) {
            throw std::invalid_argument("A arvore visual possui identificadores duplicados.");
        }
    }
    return indices;
}

void validarTopologia(const std::optional<IdentificadorRegistro>& raiz,
                      const std::vector<ModeloNoArvore>& nos,
                      const std::vector<ModeloLigacaoArvore>& ligacoes)
{
    if (nos.empty()) {
        if (raiz || !ligacoes.empty()) {
            throw std::invalid_argument(
                "Uma arvore visual vazia nao pode possuir raiz ou ligacoes.");
        }
        return;
    }

    if (!raiz) {
        throw std::invalid_argument("Uma arvore visual nao vazia deve possuir raiz.");
    }

    const auto indices = indexarNos(nos);
    const auto indiceRaiz = indices.find(*raiz);
    if (indiceRaiz == indices.end()) {
        throw std::invalid_argument("A raiz da arvore visual nao corresponde a um no.");
    }

    std::vector<std::vector<std::size_t>> filhos(nos.size());
    std::vector<std::size_t> quantidadePais(nos.size(), 0);
    std::vector<unsigned char> possuiFilhoEsquerdo(nos.size(), 0);
    std::vector<unsigned char> possuiFilhoDireito(nos.size(), 0);

    for (const auto& ligacao : ligacoes) {
        const auto pai = indices.find(ligacao.pai);
        const auto filho = indices.find(ligacao.filho);
        if (pai == indices.end() || filho == indices.end()) {
            throw std::invalid_argument(
                "Uma ligacao da arvore visual referencia um no inexistente.");
        }
        if (pai->second == filho->second) {
            throw std::invalid_argument("Um no da arvore visual nao pode ligar-se a si mesmo.");
        }

        unsigned char& ladoOcupado = ligacao.lado == LadoLigacaoArvore::Esquerdo
            ? possuiFilhoEsquerdo[pai->second]
            : possuiFilhoDireito[pai->second];
        if (ladoOcupado) {
            throw std::invalid_argument(
                "Um no da arvore visual possui mais de um filho no mesmo lado.");
        }
        ladoOcupado = 1;

        if (++quantidadePais[filho->second] > 1) {
            throw std::invalid_argument("Um no da arvore visual possui mais de um pai.");
        }
        filhos[pai->second].push_back(filho->second);
    }

    if (quantidadePais[indiceRaiz->second] != 0) {
        throw std::invalid_argument("A raiz da arvore visual nao pode possuir pai.");
    }
    for (std::size_t indice = 0; indice < nos.size(); ++indice) {
        if (indice != indiceRaiz->second && quantidadePais[indice] != 1) {
            throw std::invalid_argument(
                "Todo no que nao seja a raiz deve possuir exatamente um pai.");
        }
    }

    std::queue<std::size_t> pendentes;
    pendentes.push(indiceRaiz->second);
    std::vector<bool> visitados(nos.size(), false);
    std::size_t quantidadeVisitados = 0;
    while (!pendentes.empty()) {
        const auto indice = pendentes.front();
        pendentes.pop();
        if (visitados[indice]) {
            throw std::invalid_argument("A arvore visual possui um ciclo.");
        }
        visitados[indice] = true;
        ++quantidadeVisitados;
        for (const auto filho : filhos[indice]) {
            pendentes.push(filho);
        }
    }

    if (quantidadeVisitados != nos.size()) {
        throw std::invalid_argument("A arvore visual possui componentes desconectados.");
    }
}

} // namespace

ModeloVisualArvore::ModeloVisualArvore(
    std::optional<IdentificadorRegistro> raiz,
    std::vector<ModeloNoArvore> nos,
    std::vector<ModeloLigacaoArvore> ligacoes,
    MetricasArvore metricas)
    : raiz_(std::move(raiz)), nos_(std::move(nos)),
      ligacoes_(std::move(ligacoes)), metricas_(metricas)
{
    validarTopologia(raiz_, nos_, ligacoes_);
    if (metricas_.quantidadeNos == 0 && !nos_.empty()) {
        metricas_.quantidadeNos = nos_.size();
    }
    if (metricas_.quantidadeNos != nos_.size()) {
        throw std::invalid_argument(
            "A quantidade da metrica deve corresponder aos nos da arvore visual.");
    }
}

bool ModeloVisualArvore::vazia() const noexcept
{
    return nos_.empty();
}

const std::optional<IdentificadorRegistro>& ModeloVisualArvore::raiz() const noexcept
{
    return raiz_;
}

const std::vector<ModeloNoArvore>& ModeloVisualArvore::nos() const noexcept
{
    return nos_;
}

const std::vector<ModeloLigacaoArvore>& ModeloVisualArvore::ligacoes() const noexcept
{
    return ligacoes_;
}

const MetricasArvore& ModeloVisualArvore::metricas() const noexcept
{
    return metricas_;
}

const ModeloNoArvore* ModeloVisualArvore::buscarNo(
    const IdentificadorRegistro& identificador) const noexcept
{
    for (const auto& no : nos_) {
        if (no.identificador == identificador) {
            return &no;
        }
    }
    return nullptr;
}

} // namespace apresentacao::arvore
