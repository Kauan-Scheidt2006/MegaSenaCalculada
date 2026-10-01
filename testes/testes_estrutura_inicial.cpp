#include "dominio/Registro.hpp"
#include "estruturas/ArvoreAvl.hpp"
#include "estruturas/ArvoreBinariaBusca.hpp"

#include <cassert>
#include <memory>
#include <type_traits>
#include <vector>

namespace {

using estruturas::ArvoreBinariaBusca;
using estruturas::ArvoreAvl;
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

void verificarRotacaoAvl(const std::vector<int>& numeros)
{
    ArvoreAvl<Chave, ComparadorChave> arvore;
    for (int numero : numeros) {
        assert(arvore.inserir(Chave{1, numero}));
    }

    assert(arvore.quantidade() == 3);
    assert(arvore.altura() == 2);
    assert((arvore.percorrerEmOrdem()
            == std::vector<Chave>{{1, 10}, {1, 20}, {1, 30}}));
}

void testarRotacoesDaAvl()
{
    verificarRotacaoAvl({30, 20, 10}); // RD
    verificarRotacaoAvl({10, 20, 30}); // RE
    verificarRotacaoAvl({30, 10, 20}); // ED
    verificarRotacaoAvl({10, 30, 20}); // DE
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

} // namespace

int main()
{
    testarComparacaoLexicografica();
    testarInsercaoBuscaEPercurso();
    testarRejeicaoDeChaveDuplicada();
    testarAlturaDaArvoreNaoBalanceada();
    testarVisitaDaTopologiaEmOrdem();
    testarEstruturaInicialDaAvl();
    testarRotacoesDaAvl();
    testarDuplicataNaAvl();
    testarSequenciaOrdenadaNaAvl();
    return 0;
}
