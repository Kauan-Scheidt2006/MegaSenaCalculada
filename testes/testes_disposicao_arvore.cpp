#include "interface/componentes/arvore/CalculadorDisposicaoArvore.hpp"

#include <cassert>
#include <stdexcept>

using apresentacao::arvore::IdentificadorRegistro;
using apresentacao::arvore::LadoLigacaoArvore;
using apresentacao::arvore::ModeloVisualArvore;
using interface_grafica::arvore::CalculadorDisposicaoArvore;

namespace {

constexpr IdentificadorRegistro Raiz{2, 30};
constexpr IdentificadorRegistro Esquerdo{1, 10};
constexpr IdentificadorRegistro Meio{1, 20};
constexpr IdentificadorRegistro Direito{3, 40};

void testarDisposicaoCompacta()
{
    const ModeloVisualArvore modelo{
        Raiz,
        {{Direito}, {Raiz}, {Meio}, {Esquerdo}},
        {
            {Raiz, Esquerdo, LadoLigacaoArvore::Esquerdo},
            {Esquerdo, Meio, LadoLigacaoArvore::Direito},
            {Raiz, Direito, LadoLigacaoArvore::Direito},
        }};
    const auto disposicao = CalculadorDisposicaoArvore{}.calcular(
        modelo, {.diametroNo = 50,
                 .espacamentoHorizontal = 70,
                 .espacamentoVertical = 80});

    assert(disposicao.posicoes().size() == 4);
    assert(disposicao.posicao(Esquerdo).x() == 0);
    assert(disposicao.posicao(Meio).x() == 0);
    assert(disposicao.posicao(Raiz).x() == 35);
    assert(disposicao.posicao(Direito).x() == 70);
    assert(disposicao.posicao(Raiz).y() == 0);
    assert(disposicao.posicao(Meio).y() == 160);
    assert(disposicao.limites().width() == 170);
    assert(disposicao.limites().height() == 260);
}

void testarArvoreVaziaEParametrosInvalidos()
{
    const auto vazia = CalculadorDisposicaoArvore{}.calcular(
        ModeloVisualArvore{}, {50, 70, 80});
    assert(vazia.posicoes().empty());
    assert(vazia.limites().isNull());

    bool falhou = false;
    try {
        (void)CalculadorDisposicaoArvore{}.calcular(
            ModeloVisualArvore{}, {0, 70, 80});
    } catch (const std::invalid_argument&) {
        falhou = true;
    }
    assert(falhou);
}

void testarCadeiaCresceNaVertical()
{
    constexpr IdentificadorRegistro A{1, 1};
    constexpr IdentificadorRegistro B{2, 2};
    constexpr IdentificadorRegistro C{3, 3};
    constexpr IdentificadorRegistro D{4, 4};
    const ModeloVisualArvore modelo{
        A, {{D}, {B}, {A}, {C}},
        {{A, B, LadoLigacaoArvore::Direito},
         {B, C, LadoLigacaoArvore::Direito},
         {C, D, LadoLigacaoArvore::Direito}}};
    const auto disposicao = CalculadorDisposicaoArvore{}.calcular(
        modelo, {50, 70, 80});

    assert(disposicao.posicao(A).x() == disposicao.posicao(B).x());
    assert(disposicao.posicao(B).x() == disposicao.posicao(C).x());
    assert(disposicao.posicao(C).x() == disposicao.posicao(D).x());
    assert(disposicao.limites().width() == 100);
    assert(disposicao.limites().height() == 340);
}

} // namespace

int main()
{
    testarDisposicaoCompacta();
    testarArvoreVaziaEParametrosInvalidos();
    testarCadeiaCresceNaVertical();
    return 0;
}
