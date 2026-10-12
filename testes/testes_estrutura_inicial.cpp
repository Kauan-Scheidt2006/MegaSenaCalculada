#include "dominio/Registro.hpp"
#include "estruturas/ArvoreAvl.hpp"
#include "estruturas/ArvoreAvlPassoAPasso.hpp"
#include "estruturas/ArvoreBinariaBusca.hpp"
#include "estruturas/ArvoreRubroNegra.hpp"

#include <algorithm>
#include <cassert>
#include <memory>
#include <numeric>
#include <optional>
#include <random>
#include <set>
#include <type_traits>
#include <vector>

namespace {

using estruturas::ArvoreBinariaBusca;
using estruturas::ArvoreAvl;
using estruturas::ArvoreRubroNegra;
using estruturas::CorNoRubroNegro;
using estruturas::MetricasNavegacaoArvore;
using estruturas::NoAvl;
using megasena::Chave;
using megasena::ComparadorChave;
using megasena::ComparadorRegistro;
using megasena::Registro;

void testarComparacaoLexicografica()
{
    constexpr ComparadorChave comparar;

    static_assert(comparar(Chave{2, 60}, Chave{3, 1}));
    static_assert(!comparar(Chave{3, 1}, Chave{2, 60}));
    static_assert(comparar(Chave{5, 10}, Chave{5, 20}));
    static_assert(!comparar(Chave{5, 20}, Chave{5, 10}));
    static_assert(!comparar(Chave{5, 10}, Chave{5, 10}));

    constexpr Chave primeira{1, 60};
    constexpr Chave segunda{2, 1};
    constexpr Chave terceira{2, 30};
    static_assert(comparar(primeira, segunda));
    static_assert(comparar(segunda, terceira));
    static_assert(comparar(primeira, terceira));
}

void testarInsercaoBuscaEPercurso()
{
    ArvoreBinariaBusca<Registro, ComparadorRegistro> arvore;

    const Registro primeira{{1, 30}, 1};
    const Registro segunda{{1, 5}, 1};
    const Registro terceira{{2, 30}, 3};
    const Registro quarta{{1, 41}, 1};

    assert(arvore.vazia());
    assert(arvore.inserir(primeira));
    assert(arvore.inserir(segunda));
    assert(arvore.inserir(terceira));
    assert(arvore.inserir(quarta));
    assert(!arvore.vazia());
    assert(arvore.quantidade() == 4);

    const Registro consulta{{2, 30}, 999};
    const Registro* encontrado = arvore.buscar(consulta);
    assert(encontrado != nullptr);
    assert(encontrado->numeroConcurso == 3);
    assert(arvore.contem(Registro{{1, 5}, 500}));
    assert(!arvore.contem(Registro{{9, 60}, 500}));

    const std::vector<Registro> ordenados = arvore.percorrerEmOrdem();
    assert((ordenados[0].chave == Chave{1, 5}));
    assert((ordenados[1].chave == Chave{1, 30}));
    assert((ordenados[2].chave == Chave{1, 41}));
    assert((ordenados[3].chave == Chave{2, 30}));
}

void testarRejeicaoDeChaveDuplicada()
{
    ArvoreBinariaBusca<Registro, ComparadorRegistro> arvore;

    assert(arvore.inserir(Registro{{1, 4}, 1}));
    assert(!arvore.inserir(Registro{{1, 4}, 999}));
    assert(arvore.quantidade() == 1);

    const Registro* preservado = arvore.buscar(Registro{{1, 4}, 0});
    assert(preservado != nullptr);
    assert(preservado->numeroConcurso == 1);
}

void testarAlturaDaArvoreNaoBalanceada()
{
    ArvoreBinariaBusca<Chave, ComparadorChave> arvore;

    assert(arvore.altura() == 0);
    assert(arvore.inserir(Chave{1, 4}));
    assert(arvore.inserir(Chave{1, 5}));
    assert(arvore.inserir(Chave{1, 30}));
    assert(arvore.altura() == 3);
}

template<typename Arvore>
void testarBuscaRecursivaEPercursos()
{
    Arvore arvore;
    for (int numero : {20, 10, 30}) {
        assert(arvore.inserir(Chave{1, numero}));
    }

    const Chave* encontrada = arvore.buscar(Chave{1, 10});
    assert(encontrada != nullptr);
    assert(encontrada->numero == 10);
    assert(arvore.contem(Chave{1, 30}));
    assert(!arvore.contem(Chave{1, 99}));
    MetricasNavegacaoArvore metricas{99, 99};
    assert(arvore.buscar(Chave{1, 10}, metricas) != nullptr);
    assert(metricas.nosVisitados == 2);
    assert(metricas.comparacoes == 3);
    assert(arvore.buscar(Chave{1, 99}, metricas) == nullptr);
    assert(metricas.nosVisitados == 2);
    assert(metricas.comparacoes == 4);
    assert(arvore.metricasPeriodicasCalculadas());
    assert(arvore.metricasPeriodicas().empty());
    assert((arvore.percorrerPreOrdem()
            == std::vector<Chave>{{1, 20}, {1, 10}, {1, 30}}));
    assert((arvore.percorrerPosOrdem()
            == std::vector<Chave>{{1, 10}, {1, 30}, {1, 20}}));
}

template<typename Arvore>
void testarAlturaPeriodica()
{
    Arvore arvore;
    for (int numero = 1; numero <= 800; ++numero) {
        assert(arvore.inserir(Chave{numero, numero % 60 + 1}));
    }
    assert(arvore.metricasPeriodicasCalculadas());
    assert(arvore.metricasPeriodicas().size() == 1);
    assert(arvore.metricasPeriodicas()[0].quantidadeInserida == 800);
    assert(arvore.metricasPeriodicas()[0].altura == arvore.altura());
}

void testarVisitaDaTopologiaEmOrdem()
{
    ArvoreBinariaBusca<Chave, ComparadorChave> arvore;
    assert(arvore.inserir(Chave{2, 30}));
    assert(arvore.inserir(Chave{1, 5}));
    assert(arvore.inserir(Chave{3, 40}));

    std::vector<Chave> visitadas;
    std::vector<std::size_t> profundidades;
    const Chave* paiDaPrimeira = nullptr;
    bool primeiraEhFilhaEsquerda = false;

    arvore.visitarNosEmOrdem(
        [&](const Chave& chave, const Chave* pai, bool filhaEsquerda,
            std::size_t profundidade) {
            visitadas.push_back(chave);
            profundidades.push_back(profundidade);
            if (visitadas.size() == 1) {
                paiDaPrimeira = pai;
                primeiraEhFilhaEsquerda = filhaEsquerda;
            }
        });

    assert((visitadas == std::vector<Chave>{{1, 5}, {2, 30}, {3, 40}}));
    assert((profundidades == std::vector<std::size_t>{1, 0, 1}));
    assert(paiDaPrimeira != nullptr);
    assert((*paiDaPrimeira == Chave{2, 30}));
    assert(primeiraEhFilhaEsquerda);
}

void testarEstruturaInicialDaAvl()
{
    using No = NoAvl<Registro>;
    static_assert(std::is_same_v<decltype(No::esquerdo), std::unique_ptr<No>>);
    static_assert(std::is_same_v<decltype(No::direito), std::unique_ptr<No>>);

    No raiz(Registro{{1, 27}, 1001});
    raiz.esquerdo = std::make_unique<No>(Registro{{1, 12}, 1001});

    assert(raiz.valor.chave.numero == 27);
    assert(raiz.altura == 1);
    assert(raiz.esquerdo->valor.chave.numero == 12);
    assert(raiz.esquerdo->altura == 1);

    ArvoreAvl<Registro, ComparadorRegistro> arvore;
    assert(arvore.vazia());
    assert(arvore.quantidade() == 0);
}

void verificarRotacaoAvl(const std::vector<int>& numeros,
                         std::size_t quantidadeRotacoesEsperada)
{
    ArvoreAvl<Chave, ComparadorChave> arvore;
    for (int numero : numeros) {
        assert(arvore.inserir(Chave{1, numero}));
    }

    assert(arvore.quantidade() == 3);
    assert(arvore.altura() == 2);
    assert(arvore.quantidadeRotacoes() == quantidadeRotacoesEsperada);
    assert((arvore.percorrerEmOrdem()
            == std::vector<Chave>{{1, 10}, {1, 20}, {1, 30}}));
}

void testarRotacoesDaAvl()
{
    verificarRotacaoAvl({30, 20, 10}, 1); // RD
    verificarRotacaoAvl({10, 20, 30}, 1); // RE
    verificarRotacaoAvl({30, 10, 20}, 2); // ED
    verificarRotacaoAvl({10, 30, 20}, 2); // DE
}

void testarDuplicataNaAvl()
{
    ArvoreAvl<Registro, ComparadorRegistro> arvore;
    assert(arvore.inserir(Registro{{1, 4}, 1}));
    assert(!arvore.inserir(Registro{{1, 4}, 999}));
    assert(arvore.quantidade() == 1);
    assert(arvore.altura() == 1);

    const std::vector<Registro> registros = arvore.percorrerEmOrdem();
    assert(registros.size() == 1);
    assert(registros.front().numeroConcurso == 1);
}

void testarSequenciaOrdenadaNaAvl()
{
    ArvoreAvl<Chave, ComparadorChave> arvore;
    for (int numero = 1; numero <= 60; ++numero) {
        assert(arvore.inserir(Chave{1, numero}));
    }

    assert(arvore.quantidade() == 60);
    assert(arvore.altura() == 6);

    const std::vector<Chave> ordenadas = arvore.percorrerEmOrdem();
    for (int indice = 0; indice < 60; ++indice) {
        assert((ordenadas[static_cast<std::size_t>(indice)]
                == Chave{1, indice + 1}));
    }
}

void testarAvlPassoAPasso()
{
    const std::vector<std::vector<int>> casos{
        {30, 20, 10}, {10, 20, 30}, {30, 10, 20}, {10, 30, 20}};
    const std::vector<std::size_t> rotacoesEsperadas{1, 1, 2, 2};

    for (std::size_t caso = 0; caso < casos.size(); ++caso) {
        estruturas::ArvoreAvlPassoAPasso<Chave, ComparadorChave> didatica;
        ArvoreAvl<Chave, ComparadorChave> final;
        std::size_t eventosRotacao = 0;

        for (int numero : casos[caso]) {
            const Chave chave{1, numero};
            assert(final.inserir(chave));
            didatica.prepararInsercao(chave);
            do {
                const auto evento = didatica.avancar();
                if (evento.tipo == estruturas::TipoEventoPassoAvl::RotacaoEsquerda
                    || evento.tipo == estruturas::TipoEventoPassoAvl::RotacaoDireita) {
                    ++eventosRotacao;
                    assert(evento.raizSubarvoreAntes.has_value());
                    assert(evento.raizSubarvoreDepois.has_value());
                    assert(evento.valor == evento.raizSubarvoreDepois);
                    assert(evento.raizSubarvoreAntes
                           != evento.raizSubarvoreDepois);
                }
            } while (didatica.insercaoEmAndamento());
        }

        std::vector<Chave> ordenadas;
        std::optional<Chave> raiz;
        didatica.visitarNosEmOrdem(
            [&](const Chave& chave, const Chave* pai, bool, std::size_t) {
                ordenadas.push_back(chave);
                if (!pai) raiz = chave;
            });
        assert((ordenadas == final.percorrerEmOrdem()));
        assert((raiz == Chave{1, 20}));
        assert(didatica.altura() == final.altura());
        assert(didatica.quantidadeRotacoes() == final.quantidadeRotacoes());
        assert(eventosRotacao == rotacoesEsperadas[caso]);
    }

    estruturas::ArvoreAvlPassoAPasso<Chave, ComparadorChave> duplicata;
    duplicata.prepararInsercao(Chave{1, 10});
    while (duplicata.insercaoEmAndamento()) (void)duplicata.avancar();
    duplicata.prepararInsercao(Chave{1, 10});
    while (duplicata.insercaoEmAndamento()) (void)duplicata.avancar();
    assert(duplicata.quantidade() == 1);
}

void testarInsercaoRubroNegraPreservaInvariantes()
{
    const std::vector<std::vector<int>> sequencias{
        {30, 20, 10},
        {10, 20, 30},
        {30, 10, 20},
        {10, 30, 20},
        {7, 3, 18, 10, 22, 8, 11, 26, 2, 6, 13},
    };

    for (const auto& sequencia : sequencias) {
        ArvoreRubroNegra<Chave, ComparadorChave> arvore;
        assert(arvore.vazia());
        assert(arvore.invariantesPreservadas());

        for (int numero : sequencia) {
            assert(arvore.inserir(Chave{1, numero}));
            assert(arvore.invariantesPreservadas());
        }

        assert(!arvore.vazia());
        assert(arvore.quantidade() == sequencia.size());
        auto ordenadas = arvore.percorrerEmOrdem();
        assert(std::is_sorted(ordenadas.begin(), ordenadas.end(),
                              ComparadorChave{}));
    }
}

void testarLeituraDaTopologiaRubroNegra()
{
    ArvoreRubroNegra<Chave, ComparadorChave> arvore;
    assert(arvore.altura() == 0);
    assert(arvore.quantidadeRotacoes() == 0);

    assert(arvore.inserir(Chave{1, 30}));
    assert(arvore.inserir(Chave{1, 20}));
    assert(arvore.inserir(Chave{1, 10}));

    struct NoVisitado {
        int numero;
        int numeroPai;
        bool filhoEsquerdo;
        std::size_t profundidade;
        CorNoRubroNegro cor;
    };
    std::vector<NoVisitado> visitados;
    arvore.visitarNosComCorEmOrdem(
        [&](const Chave& chave, const Chave* pai, bool filhoEsquerdo,
            std::size_t profundidade, CorNoRubroNegro cor) {
            visitados.push_back(
                {chave.numero, pai ? pai->numero : 0,
                 filhoEsquerdo, profundidade, cor});
        });

    assert(arvore.altura() == 2);
    assert(arvore.quantidadeRotacoes() == 1);
    assert(visitados.size() == 3);
    assert(visitados[0].numero == 10);
    assert(visitados[0].numeroPai == 20);
    assert(visitados[0].filhoEsquerdo);
    assert(visitados[0].profundidade == 1);
    assert(visitados[0].cor == CorNoRubroNegro::Vermelho);
    assert(visitados[1].numero == 20);
    assert(visitados[1].numeroPai == 0);
    assert(visitados[1].profundidade == 0);
    assert(visitados[1].cor == CorNoRubroNegro::Preto);
    assert(visitados[2].numero == 30);
    assert(visitados[2].numeroPai == 20);
    assert(!visitados[2].filhoEsquerdo);
    assert(visitados[2].profundidade == 1);
    assert(visitados[2].cor == CorNoRubroNegro::Vermelho);
}

void testarCasosTradicionaisDaInsercaoRubroNegra()
{
    const std::vector<std::vector<int>> casos{
        {30, 20, 10}, {10, 20, 30}, {30, 10, 20}, {10, 30, 20}};
    const std::vector<std::size_t> rotacoesEsperadas{1, 1, 2, 2};

    for (std::size_t indice = 0; indice < casos.size(); ++indice) {
        ArvoreRubroNegra<Chave, ComparadorChave> arvore;
        for (int numero : casos[indice]) {
            const auto rotacoesAntes = arvore.quantidadeRotacoes();
            assert(arvore.inserir(Chave{1, numero}));
            assert(arvore.quantidadeRotacoes() - rotacoesAntes <= 2);
            assert(arvore.invariantesPreservadas());
        }

        std::vector<std::pair<int, CorNoRubroNegro>> nos;
        arvore.visitarNosComCorEmOrdem(
            [&](const Chave& chave, const Chave*, bool, std::size_t,
                CorNoRubroNegro cor) {
                nos.emplace_back(chave.numero, cor);
            });

        assert(arvore.quantidadeRotacoes() == rotacoesEsperadas[indice]);
        assert(nos.size() == 3);
        assert(nos[0] == std::make_pair(10, CorNoRubroNegro::Vermelho));
        assert(nos[1] == std::make_pair(20, CorNoRubroNegro::Preto));
        assert(nos[2] == std::make_pair(30, CorNoRubroNegro::Vermelho));
    }

    ArvoreRubroNegra<Chave, ComparadorChave> recoloracao;
    for (int numero : {20, 10, 30}) {
        assert(recoloracao.inserir(Chave{1, numero}));
    }
    const auto rotacoesAntes = recoloracao.quantidadeRotacoes();
    assert(recoloracao.inserir(Chave{1, 5}));
    assert(recoloracao.quantidadeRotacoes() == rotacoesAntes);
    assert(recoloracao.invariantesPreservadas());

    std::vector<std::pair<int, CorNoRubroNegro>> nos;
    recoloracao.visitarNosComCorEmOrdem(
        [&](const Chave& chave, const Chave*, bool, std::size_t,
            CorNoRubroNegro cor) {
            nos.emplace_back(chave.numero, cor);
        });
    assert(nos.size() == 4);
    assert(nos[0] == std::make_pair(5, CorNoRubroNegro::Vermelho));
    assert(nos[1] == std::make_pair(10, CorNoRubroNegro::Preto));
    assert(nos[2] == std::make_pair(20, CorNoRubroNegro::Preto));
    assert(nos[3] == std::make_pair(30, CorNoRubroNegro::Preto));
}

void testarVisitaCompativelEContagemDeRotacaoDuplaRubroNegra()
{
    ArvoreRubroNegra<Chave, ComparadorChave> arvore;
    assert(arvore.inserir(Chave{1, 10}));
    assert(arvore.inserir(Chave{1, 30}));
    assert(arvore.inserir(Chave{1, 20}));

    std::vector<int> ordem;
    arvore.visitarNosEmOrdem(
        [&](const Chave& chave, const Chave*, bool, std::size_t) {
            ordem.push_back(chave.numero);
        });

    assert((ordem == std::vector<int>{10, 20, 30}));
    assert(arvore.altura() == 2);
    assert(arvore.quantidadeRotacoes() == 2);
    assert(arvore.invariantesPreservadas());
}

void testarDuplicataNaRubroNegra()
{
    ArvoreRubroNegra<Registro, ComparadorRegistro> arvore;
    assert(arvore.inserir(Registro{{1, 4}, 1}));
    const auto antes = arvore.percorrerEmOrdem();
    const auto rotacoesAntes = arvore.quantidadeRotacoes();

    assert(!arvore.inserir(Registro{{1, 4}, 999}));
    assert(arvore.quantidade() == 1);
    const auto depois = arvore.percorrerEmOrdem();
    assert(depois.size() == antes.size());
    assert(depois.front().chave == antes.front().chave);
    assert(depois.front().numeroConcurso == antes.front().numeroConcurso);
    assert(depois.front().numeroConcurso == 1);
    assert(arvore.quantidadeRotacoes() == rotacoesAntes);
    assert(arvore.invariantesPreservadas());
}

void testarSequenciaExtensaNaRubroNegra()
{
    ArvoreRubroNegra<Chave, ComparadorChave> arvore;
    for (int numero = 1; numero <= 1000; ++numero) {
        assert(arvore.inserir(Chave{1, numero}));
        assert(arvore.invariantesPreservadas());
    }

    assert(arvore.quantidade() == 1000);
    const auto ordenadas = arvore.percorrerEmOrdem();
    assert(ordenadas.front() == (Chave{1, 1}));
    assert(ordenadas.back() == (Chave{1, 1000}));
}

void testarPermutacoesDeterministicasNaRubroNegra()
{
    std::vector<int> numeros(128);
    std::iota(numeros.begin(), numeros.end(), 1);
    std::mt19937 gerador(20261008);

    for (int repeticao = 0; repeticao < 50; ++repeticao) {
        std::shuffle(numeros.begin(), numeros.end(), gerador);
        ArvoreRubroNegra<Chave, ComparadorChave> arvore;
        for (int numero : numeros) {
            const auto rotacoesAntes = arvore.quantidadeRotacoes();
            assert(arvore.inserir(Chave{1, numero}));
            assert(arvore.quantidadeRotacoes() - rotacoesAntes <= 2);
            assert(arvore.invariantesPreservadas());
        }
        assert(arvore.quantidade() == numeros.size());
    }
}

template<typename Arvore>
void testarRemocoesBasicas()
{
    Arvore arvore;
    MetricasNavegacaoArvore metricas{99, 99};
    assert(!arvore.remover(Chave{1, 10}, metricas));
    assert(metricas.nosVisitados == 0);
    assert(metricas.comparacoes == 0);

    for (int numero : {40, 20, 60, 10, 30, 50, 70, 25}) {
        assert(arvore.inserir(Chave{1, numero}));
    }

    assert(arvore.remover(Chave{1, 10}, metricas));
    assert(metricas.nosVisitados == 3);
    assert(metricas.comparacoes == 4);
    assert(arvore.remover(Chave{1, 30}, metricas));
    assert(metricas.nosVisitados > 0);
    assert(metricas.comparacoes > 0);
    assert(arvore.remover(Chave{1, 60}, metricas));
    assert(metricas.nosVisitados > 0);
    assert(metricas.comparacoes > 0);
    assert(arvore.remover(Chave{1, 40}, metricas));
    assert(metricas.nosVisitados > 0);
    assert(metricas.comparacoes > 0);
    assert(!arvore.remover(Chave{1, 99}, metricas));
    assert(metricas.nosVisitados > 0);
    assert(metricas.comparacoes > 0);
    assert(arvore.quantidade() == 4);
    assert((arvore.percorrerEmOrdem()
            == std::vector<Chave>{{1, 20}, {1, 25}, {1, 50}, {1, 70}}));

    for (int numero : {20, 25, 50, 70}) {
        assert(arvore.remover(Chave{1, numero}));
    }
    assert(arvore.vazia());
    assert(arvore.quantidade() == 0);
}

template<typename Arvore>
void testarRemocaoContabilizaSucessor()
{
    Arvore arvore;
    for (int numero : {20, 10, 30}) {
        assert(arvore.inserir(Chave{1, numero}));
    }

    MetricasNavegacaoArvore metricas{99, 99};
    assert(arvore.remover(Chave{1, 20}, metricas));
    assert(metricas.nosVisitados == 2);
    assert(metricas.comparacoes == 2);
}

void testarRemocoesAvlPreservamBalanceamento()
{
    ArvoreAvl<Chave, ComparadorChave> arvore;
    for (int numero = 1; numero <= 200; ++numero) {
        assert(arvore.inserir(Chave{1, numero}));
    }
    for (int numero = 1; numero <= 200; numero += 2) {
        assert(arvore.remover(Chave{1, numero}));
        assert(arvore.altura() <= 8);
    }
    for (int numero = 2; numero <= 200; numero += 2) {
        assert(arvore.remover(Chave{1, numero}));
    }
    assert(arvore.vazia());
}

void testarRemocoesRubroNegrasContraConjuntoOrdenado()
{
    std::vector<int> numeros(256);
    std::iota(numeros.begin(), numeros.end(), 1);
    std::mt19937 gerador(20261010);
    std::shuffle(numeros.begin(), numeros.end(), gerador);

    ArvoreRubroNegra<Chave, ComparadorChave> arvore;
    std::set<int> referencia;
    for (int numero : numeros) {
        assert(arvore.inserir(Chave{1, numero}));
        referencia.insert(numero);
    }

    std::shuffle(numeros.begin(), numeros.end(), gerador);
    for (int numero : numeros) {
        const auto rotacoesAntes = arvore.quantidadeRotacoes();
        assert(arvore.remover(Chave{1, numero}));
        assert(arvore.quantidadeRotacoes() - rotacoesAntes <= 3);
        referencia.erase(numero);
        assert(arvore.invariantesPreservadas());
        assert(arvore.quantidade() == referencia.size());
        const auto valores = arvore.percorrerEmOrdem();
        assert(valores.size() == referencia.size());
        auto esperado = referencia.begin();
        for (const auto& valor : valores) {
            assert(esperado != referencia.end());
            assert(valor.numero == *esperado++);
        }
    }
    assert(arvore.vazia());
}

} // namespace

int main()
{
    testarComparacaoLexicografica();
    testarInsercaoBuscaEPercurso();
    testarRejeicaoDeChaveDuplicada();
    testarAlturaDaArvoreNaoBalanceada();
    testarBuscaRecursivaEPercursos<
        ArvoreBinariaBusca<Chave, ComparadorChave>>();
    testarBuscaRecursivaEPercursos<ArvoreAvl<Chave, ComparadorChave>>();
    testarBuscaRecursivaEPercursos<
        ArvoreRubroNegra<Chave, ComparadorChave>>();
    testarAlturaPeriodica<
        ArvoreBinariaBusca<Chave, ComparadorChave>>();
    testarAlturaPeriodica<ArvoreAvl<Chave, ComparadorChave>>();
    testarAlturaPeriodica<
        ArvoreRubroNegra<Chave, ComparadorChave>>();
    testarVisitaDaTopologiaEmOrdem();
    testarEstruturaInicialDaAvl();
    testarRotacoesDaAvl();
    testarDuplicataNaAvl();
    testarSequenciaOrdenadaNaAvl();
    testarAvlPassoAPasso();
    testarInsercaoRubroNegraPreservaInvariantes();
    testarLeituraDaTopologiaRubroNegra();
    testarCasosTradicionaisDaInsercaoRubroNegra();
    testarVisitaCompativelEContagemDeRotacaoDuplaRubroNegra();
    testarDuplicataNaRubroNegra();
    testarSequenciaExtensaNaRubroNegra();
    testarPermutacoesDeterministicasNaRubroNegra();
    testarRemocoesBasicas<
        ArvoreBinariaBusca<Chave, ComparadorChave>>();
    testarRemocoesBasicas<ArvoreAvl<Chave, ComparadorChave>>();
    testarRemocoesBasicas<
        ArvoreRubroNegra<Chave, ComparadorChave>>();
    testarRemocaoContabilizaSucessor<
        ArvoreBinariaBusca<Chave, ComparadorChave>>();
    testarRemocaoContabilizaSucessor<
        ArvoreAvl<Chave, ComparadorChave>>();
    testarRemocaoContabilizaSucessor<
        ArvoreRubroNegra<Chave, ComparadorChave>>();
    testarRemocoesAvlPreservamBalanceamento();
    testarRemocoesRubroNegrasContraConjuntoOrdenado();
    return 0;
}
