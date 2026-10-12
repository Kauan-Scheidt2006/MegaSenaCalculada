#include "interface/apresentacao/arvore/DadosDetalhesRegistro.hpp"
#include "interface/apresentacao/arvore/ModeloApresentacaoArvore.hpp"

#include <cassert>
#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>

using apresentacao::arvore::DadosDetalhesRegistro;
using apresentacao::arvore::EstadoVisualizacaoArvore;
using apresentacao::arvore::IdentificadorRegistro;
using apresentacao::arvore::LadoLigacaoArvore;
using apresentacao::arvore::ModeloApresentacaoArvore;
using apresentacao::arvore::ModeloLigacaoArvore;
using apresentacao::arvore::ModeloNoArvore;
using apresentacao::arvore::ModeloVisualArvore;
using apresentacao::arvore::SemanticaVisualNo;

namespace {

constexpr IdentificadorRegistro Raiz{1, 30};
constexpr IdentificadorRegistro Esquerdo{1, 5};
constexpr IdentificadorRegistro Direito{2, 30};
constexpr IdentificadorRegistro Outro{3, 41};

template<typename Acao>
void exigirArgumentoInvalido(Acao acao)
{
    bool lancou = false;
    try {
        acao();
    } catch (const std::invalid_argument&) {
        lancou = true;
    }
    assert(lancou);
}

ModeloVisualArvore criarArvoreValida()
{
    return ModeloVisualArvore{
        Raiz,
        {{Raiz}, {Esquerdo}, {Direito}},
        {
            {Raiz, Esquerdo, LadoLigacaoArvore::Esquerdo},
            {Raiz, Direito, LadoLigacaoArvore::Direito},
        },
    };
}

void testarIdentidadeEDetalhes()
{
    constexpr IdentificadorRegistro primeira{7, 42};
    constexpr IdentificadorRegistro reconstruida{7, 42};
    constexpr IdentificadorRegistro diferente{7, 43};
    static_assert(primeira == reconstruida);
    static_assert(primeira != diferente);

    const DadosDetalhesRegistro detalhes{
        primeira,
        2890,
        QStringLiteral("05/10/2026"),
        {4, 11, 27, 42, 53, 60},
    };
    assert(detalhes.identificador.numero == 42);
    assert(detalhes.identificador.frequenciaAcumulada == 7);
    assert(detalhes.numeroConcurso == 2890);
    assert(detalhes.dataConcurso == QStringLiteral("05/10/2026"));
    assert(detalhes.dezenas.size() == 6);
    assert(detalhes.dezenas[3] == 42);
}

void testarArvoresValidasEConsultas()
{
    const ModeloVisualArvore vazia;
    assert(vazia.vazia());
    assert(!vazia.raiz());
    assert(vazia.nos().empty());
    assert(vazia.ligacoes().empty());

    const ModeloVisualArvore unica{Raiz, {{Raiz}}, {}};
    assert(!unica.vazia());
    assert(unica.raiz() == Raiz);
    assert(unica.buscarNo(Raiz) != nullptr);
    assert(unica.buscarNo(Outro) == nullptr);

    const auto arvore = criarArvoreValida();
    assert(arvore.nos().size() == 3);
    assert(arvore.ligacoes().size() == 2);
    assert(arvore.ligacoes()[0].lado == LadoLigacaoArvore::Esquerdo);
    assert(arvore.ligacoes()[1].lado == LadoLigacaoArvore::Direito);

    const ModeloVisualArvore rubroNegra{
        Raiz,
        {{Raiz, SemanticaVisualNo::RubroNegroPreto},
         {Esquerdo, SemanticaVisualNo::RubroNegroVermelho}},
        {{Raiz, Esquerdo, LadoLigacaoArvore::Esquerdo}}};
    assert(rubroNegra.buscarNo(Raiz)->semanticaVisual
           == SemanticaVisualNo::RubroNegroPreto);
    assert(rubroNegra.buscarNo(Esquerdo)->semanticaVisual
           == SemanticaVisualNo::RubroNegroVermelho);
    assert(unica.buscarNo(Raiz)->semanticaVisual
           == SemanticaVisualNo::Padrao);
}

void testarTopologiasInvalidas()
{
    exigirArgumentoInvalido([] {
        ModeloVisualArvore modelo{Raiz, {}, {}};
    });
    exigirArgumentoInvalido([] {
        ModeloVisualArvore modelo{std::nullopt, {{Raiz}}, {}};
    });
    exigirArgumentoInvalido([] {
        ModeloVisualArvore modelo{Raiz, {{Raiz}, {Raiz}}, {}};
    });
    exigirArgumentoInvalido([] {
        ModeloVisualArvore modelo{Outro, {{Raiz}}, {}};
    });
    exigirArgumentoInvalido([] {
        ModeloVisualArvore modelo{
            Raiz, {{Raiz}}, {{Raiz, Outro, LadoLigacaoArvore::Esquerdo}}};
    });
    exigirArgumentoInvalido([] {
        ModeloVisualArvore modelo{
            Raiz, {{Raiz}}, {{Raiz, Raiz, LadoLigacaoArvore::Esquerdo}}};
    });
    exigirArgumentoInvalido([] {
        ModeloVisualArvore modelo{
            Raiz,
            {{Raiz}, {Esquerdo}, {Direito}},
            {
                {Raiz, Esquerdo, LadoLigacaoArvore::Esquerdo},
                {Raiz, Direito, LadoLigacaoArvore::Esquerdo},
            }};
    });
    exigirArgumentoInvalido([] {
        ModeloVisualArvore modelo{
            Raiz,
            {{Raiz}, {Esquerdo}, {Direito}},
            {
                {Raiz, Direito, LadoLigacaoArvore::Direito},
                {Esquerdo, Direito, LadoLigacaoArvore::Direito},
            }};
    });
    exigirArgumentoInvalido([] {
        ModeloVisualArvore modelo{
            Raiz,
            {{Raiz}, {Esquerdo}},
            {{Esquerdo, Raiz, LadoLigacaoArvore::Direito}}};
    });
    exigirArgumentoInvalido([] {
        ModeloVisualArvore modelo{Raiz, {{Raiz}, {Esquerdo}}, {}};
    });
    exigirArgumentoInvalido([] {
        ModeloVisualArvore modelo{
            Raiz,
            {{Raiz}, {Esquerdo}, {Direito}},
            {
                {Esquerdo, Direito, LadoLigacaoArvore::Direito},
                {Direito, Esquerdo, LadoLigacaoArvore::Esquerdo},
            }};
    });
}

void testarEstadosELimpezaDeDados()
{
    ModeloApresentacaoArvore modelo;
    assert(modelo.estado() == EstadoVisualizacaoArvore::Vazio);
    assert(modelo.modeloVisual() == nullptr);
    assert(modelo.mensagemErro().isEmpty());

    modelo.definirConstruindo();
    assert(modelo.estado() == EstadoVisualizacaoArvore::Construindo);

    modelo.definirDisponivel(criarArvoreValida());
    assert(modelo.estado() == EstadoVisualizacaoArvore::Disponivel);
    assert(modelo.modeloVisual() != nullptr);
    assert(modelo.mensagemErro().isEmpty());

    modelo.definirErro(QStringLiteral("Falha ao construir a arvore."));
    assert(modelo.estado() == EstadoVisualizacaoArvore::Erro);
    assert(modelo.modeloVisual() == nullptr);
    assert(modelo.mensagemErro() == QStringLiteral("Falha ao construir a arvore."));

    modelo.definirCancelado();
    assert(modelo.estado() == EstadoVisualizacaoArvore::Cancelado);
    assert(modelo.modeloVisual() == nullptr);
    assert(modelo.mensagemErro().isEmpty());

    modelo.definirVazio();
    assert(modelo.estado() == EstadoVisualizacaoArvore::Vazio);

    exigirArgumentoInvalido([&modelo] {
        modelo.definirDisponivel(ModeloVisualArvore{});
    });
    exigirArgumentoInvalido([&modelo] {
        modelo.definirErro(QStringLiteral("   "));
    });
}

} // namespace

int main()
{
    testarIdentidadeEDetalhes();
    testarArvoresValidasEConsultas();
    testarTopologiasInvalidas();
    testarEstadosELimpezaDeDados();
    return 0;
}
