#include "interface/componentes/arvore/CalculadorDisposicaoArvore.hpp"

#include <algorithm>
#include <limits>
#include <map>
#include <stdexcept>
#include <utility>
#include <vector>

namespace interface_grafica::arvore {
namespace {

using Identificador = apresentacao::arvore::IdentificadorRegistro;

struct FilhosNo {
    const Identificador* esquerdo = nullptr;
    const Identificador* direito = nullptr;
};

struct SubarvoreDisposta {
    std::map<Identificador, QPointF> posicoes;
    std::vector<qreal> contornoEsquerdo;
    std::vector<qreal> contornoDireito;
    qreal raizX = 0;
};

void deslocar(SubarvoreDisposta& subarvore, qreal deltaX, qreal deltaY)
{
    for (auto& [identificador, posicao] : subarvore.posicoes) {
        (void)identificador;
        posicao += QPointF(deltaX, deltaY);
    }
    for (auto& x : subarvore.contornoEsquerdo) x += deltaX;
    for (auto& x : subarvore.contornoDireito) x += deltaX;
    subarvore.raizX += deltaX;
}

SubarvoreDisposta disporSubarvore(
    const Identificador& identificador,
    const std::map<Identificador, FilhosNo>& filhos,
    qreal separacaoHorizontal, qreal espacamentoVertical)
{
    const auto& descendentes = filhos.at(identificador);
    if (!descendentes.esquerdo && !descendentes.direito) {
        return {{{identificador, QPointF(0, 0)}}, {0}, {0}, 0};
    }

    if (!descendentes.esquerdo || !descendentes.direito) {
        const Identificador& filho = descendentes.esquerdo
            ? *descendentes.esquerdo : *descendentes.direito;
        auto resultado = disporSubarvore(
            filho, filhos, separacaoHorizontal, espacamentoVertical);
        deslocar(resultado, -resultado.raizX, espacamentoVertical);
        resultado.posicoes.emplace(identificador, QPointF(0, 0));
        resultado.contornoEsquerdo.insert(resultado.contornoEsquerdo.begin(), 0);
        resultado.contornoDireito.insert(resultado.contornoDireito.begin(), 0);
        resultado.raizX = 0;
        return resultado;
    }

    auto esquerda = disporSubarvore(
        *descendentes.esquerdo, filhos, separacaoHorizontal,
        espacamentoVertical);
    auto direita = disporSubarvore(
        *descendentes.direito, filhos, separacaoHorizontal,
        espacamentoVertical);
    deslocar(esquerda, -esquerda.raizX, espacamentoVertical);
    deslocar(direita, -direita.raizX, espacamentoVertical);

    qreal deslocamentoDireita = separacaoHorizontal;
    const std::size_t niveisComuns = std::min(
        esquerda.contornoDireito.size(), direita.contornoEsquerdo.size());
    for (std::size_t nivel = 0; nivel < niveisComuns; ++nivel) {
        deslocamentoDireita = std::max(
            deslocamentoDireita,
            esquerda.contornoDireito[nivel]
                - direita.contornoEsquerdo[nivel] + separacaoHorizontal);
    }
    deslocar(direita, deslocamentoDireita, 0);

    const qreal raizX = (esquerda.raizX + direita.raizX) / 2.0;
    SubarvoreDisposta resultado;
    resultado.raizX = raizX;
    resultado.posicoes = std::move(esquerda.posicoes);
    resultado.posicoes.insert(direita.posicoes.begin(), direita.posicoes.end());
    resultado.posicoes.emplace(identificador, QPointF(raizX, 0));

    const std::size_t niveis = std::max(
        esquerda.contornoEsquerdo.size(), direita.contornoEsquerdo.size());
    resultado.contornoEsquerdo.reserve(niveis + 1);
    resultado.contornoDireito.reserve(niveis + 1);
    resultado.contornoEsquerdo.push_back(raizX);
    resultado.contornoDireito.push_back(raizX);
    for (std::size_t nivel = 0; nivel < niveis; ++nivel) {
        const qreal minE = nivel < esquerda.contornoEsquerdo.size()
            ? esquerda.contornoEsquerdo[nivel]
            : std::numeric_limits<qreal>::max();
        const qreal minD = nivel < direita.contornoEsquerdo.size()
            ? direita.contornoEsquerdo[nivel]
            : std::numeric_limits<qreal>::max();
        const qreal maxE = nivel < esquerda.contornoDireito.size()
            ? esquerda.contornoDireito[nivel]
            : std::numeric_limits<qreal>::lowest();
        const qreal maxD = nivel < direita.contornoDireito.size()
            ? direita.contornoDireito[nivel]
            : std::numeric_limits<qreal>::lowest();
        resultado.contornoEsquerdo.push_back(std::min(minE, minD));
        resultado.contornoDireito.push_back(std::max(maxE, maxD));
    }
    return resultado;
}

} // namespace

DisposicaoArvore::DisposicaoArvore(
    std::map<Identificador, QPointF> posicoes, QRectF limites)
    : posicoes_(std::move(posicoes)), limites_(limites)
{
}

const QPointF& DisposicaoArvore::posicao(const Identificador& identificador) const
{
    const auto posicao = posicoes_.find(identificador);
    if (posicao == posicoes_.end()) {
        throw std::out_of_range("O no nao pertence a disposicao da arvore.");
    }
    return posicao->second;
}

const std::map<Identificador, QPointF>& DisposicaoArvore::posicoes() const noexcept
{
    return posicoes_;
}

const QRectF& DisposicaoArvore::limites() const noexcept { return limites_; }

DisposicaoArvore CalculadorDisposicaoArvore::calcular(
    const apresentacao::arvore::ModeloVisualArvore& modelo,
    ParametrosDisposicaoArvore parametros) const
{
    if (parametros.diametroNo <= 0 || parametros.espacamentoHorizontal <= 0
        || parametros.espacamentoVertical <= 0) {
        throw std::invalid_argument(
            "Os parametros da disposicao da arvore devem ser positivos.");
    }
    if (modelo.vazia()) return {{}, {}};

    std::map<Identificador, FilhosNo> filhos;
    for (const auto& no : modelo.nos()) filhos.emplace(no.identificador, FilhosNo{});
    for (const auto& ligacao : modelo.ligacoes()) {
        auto& filhosDoPai = filhos.at(ligacao.pai);
        if (ligacao.lado == apresentacao::arvore::LadoLigacaoArvore::Esquerdo) {
            filhosDoPai.esquerdo = &ligacao.filho;
        } else {
            filhosDoPai.direito = &ligacao.filho;
        }
    }

    const qreal separacaoCentros = std::max(
        parametros.espacamentoHorizontal, parametros.diametroNo);
    auto resultado = disporSubarvore(
        *modelo.raiz(), filhos, separacaoCentros,
        parametros.espacamentoVertical);

    qreal minimoX = std::numeric_limits<qreal>::max();
    qreal maximoX = std::numeric_limits<qreal>::lowest();
    qreal maximoY = 0;
    for (const auto& [identificador, posicao] : resultado.posicoes) {
        (void)identificador;
        minimoX = std::min(minimoX, posicao.x());
        maximoX = std::max(maximoX, posicao.x());
        maximoY = std::max(maximoY, posicao.y());
    }
    const qreal margem = parametros.diametroNo;
    return {std::move(resultado.posicoes),
            QRectF(minimoX - margem, -margem,
                   maximoX - minimoX + 2 * margem,
                   maximoY + 2 * margem)};
}

} // namespace interface_grafica::arvore
