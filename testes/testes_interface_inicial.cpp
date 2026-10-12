#include "interface/ambiente/Ambiente.hpp"
#include "interface/ambiente/AreaPaginas.hpp"
#include "interface/ambiente/BarraEstado.hpp"
#include "interface/componentes/basicos/BotaoNavegacao.hpp"
#include "interface/componentes/basicos/IndicadorProgresso.hpp"
#include "interface/componentes/basicos/Texto.hpp"
#include "interface/componentes/arvore/VisualizacaoArvore.hpp"
#include "interface/componentes/arvore/MapaArvore.hpp"
#include "interface/componentes/arvore/ItemNoArvore.hpp"
#include "interface/componentes/arvore/CampoDetalhe.hpp"
#include "interface/componentes/arvore/MarcadorDezena.hpp"
#include "interface/componentes/arvore/PopoverDetalhesRegistro.hpp"
#include "interface/componentes/arvore/ResultadoOperacaoArvore.hpp"
#include "interface/estilos/EstiloAplicacao.hpp"
#include "interface/fundamentos/BaseContainer.hpp"
#include "interface/fundamentos/BasePagina.hpp"
#include "interface/paginas/arvore/PaginaArvore.hpp"
#include "interface/paginas/arvore/ItemNavegacaoEstrutura.hpp"
#include "interface/paginas/arvore/MenuEstruturas.hpp"
#include "interface/paginas/arvore/TipoEstrutura.hpp"

#include <QApplication>
#include <QGraphicsScene>
#include <QLabel>
#include <QKeyEvent>
#include <QLineEdit>
#include <QLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QToolButton>
#include <QStringList>

#include <stdexcept>

namespace {

class ContainerTeste final : public BaseContainer
{
public:
    explicit ContainerTeste(TipoDisposicao disposicao)
        : BaseContainer({.componente = {.nomeObjeto = QStringLiteral("containerTeste"),
                                        .fundoEstilizado = false},
                         .disposicao = disposicao})
    {
    }

    using BaseContainer::adicionarComponente;
    using BaseContainer::adicionarComponenteNaGrade;
    using BaseContainer::quantidadeComponentes;
};

class PaginaTeste final : public BasePagina
{
public:
    PaginaTeste()
        : BasePagina({.componente = {.nomeObjeto = QStringLiteral("paginaTeste"),
                                     .fundoEstilizado = false},
                      .disposicao = TipoDisposicao::Vertical})
    {
    }
};

bool testarComponentesBasicos()
{
    Texto texto(QStringLiteral("Inicial"),
                {.nomeObjeto = QStringLiteral("textoTeste"), .fundoEstilizado = false});
    texto.definirConteudo(QStringLiteral("Atualizado"));
    if (texto.text() != QStringLiteral("Atualizado") || texto.accessibleName() != QStringLiteral("Atualizado")) {
        return false;
    }

    BotaoNavegacao navegacao(QStringLiteral("AVL"),
                             {.nomeObjeto = QStringLiteral("navegacaoTeste"), .fundoEstilizado = false});
    navegacao.definirSelecionado(true);
    if (!navegacao.estaSelecionado() || navegacao.focusPolicy() != Qt::StrongFocus) {
        return false;
    }

    IndicadorProgresso progresso({.nomeObjeto = QStringLiteral("progressoTeste"),
                                  .fundoEstilizado = false});
    progresso.definirProgresso(3, 10);
    if (progresso.value() != 3 || progresso.maximum() != 10) {
        return false;
    }

    try {
        progresso.definirProgresso(11, 10);
        return false;
    } catch (const std::invalid_argument&) {
    }
    return true;
}

bool testarContainers()
{
    ContainerTeste linear(TipoDisposicao::Vertical);
    Texto primeiro(QStringLiteral("Primeiro"),
                   {.nomeObjeto = QStringLiteral("primeiroTeste"), .fundoEstilizado = false}, &linear);
    linear.adicionarComponente(primeiro);
    if (linear.quantidadeComponentes() != 1) return false;

    ContainerTeste grade(TipoDisposicao::Grade);
    Texto celula(QStringLiteral("Celula"),
                 {.nomeObjeto = QStringLiteral("celulaTeste"), .fundoEstilizado = false}, &grade);
    grade.adicionarComponenteNaGrade(celula, 0, 0);
    if (grade.quantidadeComponentes() != 1) return false;

    try {
        grade.adicionarComponente(celula);
        return false;
    } catch (const std::logic_error&) {
    }
    return true;
}

bool testarContratoVisualObrigatorio()
{
    try {
        Texto semNome(QStringLiteral("Sem nome"), {.fundoEstilizado = false});
        return false;
    } catch (const std::invalid_argument&) {
    }
    try {
        Texto semFundo(QStringLiteral("Sem fundo"), {.nomeObjeto = QStringLiteral("semFundo")});
        return false;
    } catch (const std::invalid_argument&) {
    }
    return true;
}

bool testarPaginaArvore()
{
    PaginaArvore pagina;
    pagina.resize(1200, 760);
    auto* menu = pagina.findChild<MenuEstruturas*>(QStringLiteral("menuEstruturas"));
    auto* itemBuscaBinaria = dynamic_cast<ItemNavegacaoEstrutura*>(
        pagina.findChild<QWidget*>(QStringLiteral("itemBuscaBinaria")));
    auto* itemAvl = dynamic_cast<ItemNavegacaoEstrutura*>(
        pagina.findChild<QWidget*>(QStringLiteral("itemAvl")));
    auto* itemRubroNegra = dynamic_cast<ItemNavegacaoEstrutura*>(
        pagina.findChild<QWidget*>(QStringLiteral("itemRubroNegra")));
    auto* itemAvlPassoAPasso = dynamic_cast<ItemNavegacaoEstrutura*>(
        pagina.findChild<QWidget*>(QStringLiteral("itemAvlPassoAPasso")));
    const auto* conteudo = pagina.findChild<QWidget*>(QStringLiteral("conteudoPaginaArvore"));
    const auto* cabecalho = pagina.findChild<QWidget*>(QStringLiteral("cabecalhoPaginaArvore"));
    const auto* titulo = pagina.findChild<QLabel*>(QStringLiteral("tituloPaginaArvore"));
    const auto* subtitulo = pagina.findChild<QLabel*>(QStringLiteral("subtituloPaginaArvore"));
    const auto* painelVisualizacao = pagina.findChild<QWidget*>(QStringLiteral("painelVisualizacaoArvore"));
    auto* visualizacao = pagina.findChild<VisualizacaoArvore*>(QStringLiteral("visualizacaoArvore"));
    auto* mapa = pagina.findChild<MapaArvore*>(QStringLiteral("mapaArvore"));
    auto* aumentarZoom = pagina.findChild<QPushButton*>(QStringLiteral("aumentarZoomArvore"));
    auto* percentualZoom = pagina.findChild<QLabel*>(QStringLiteral("percentualZoomArvore"));
    auto* modoArrasto = pagina.findChild<QPushButton*>(QStringLiteral("modoArrastoArvore"));
    auto* modoFoco = pagina.findChild<QPushButton*>(QStringLiteral("modoFocoArvore"));
    auto* recolherSelecao = pagina.findChild<QPushButton*>(QStringLiteral("recolherSelecaoArvore"));
    auto* expandirTudo = pagina.findChild<QPushButton*>(QStringLiteral("expandirTodaArvore"));
    auto* percorrerEmOrdem = pagina.findChild<QPushButton*>(
        QStringLiteral("percorrerEmOrdemArvore"));
    auto* percorrerPreOrdem = pagina.findChild<QPushButton*>(
        QStringLiteral("percorrerPreOrdemArvore"));
    auto* percorrerPosOrdem = pagina.findChild<QPushButton*>(
        QStringLiteral("percorrerPosOrdemArvore"));
    auto* popover = pagina.findChild<PopoverDetalhesRegistro*>(
        QStringLiteral("popoverDetalhesRegistro"));
    auto* fecharPopover = pagina.findChild<QPushButton*>(
        QStringLiteral("fecharPopoverDetalhes"));
    auto* controleExecucao = pagina.findChild<QWidget*>(
        QStringLiteral("controleExecucaoAvl"));
    auto* proximoPasso = pagina.findChild<QPushButton*>(
        QStringLiteral("proximoPassoAvl"));
    auto* resultadoOperacao = pagina.findChild<ResultadoOperacaoArvore*>(
        QStringLiteral("resultadoOperacaoArvore"));
    auto* resumoResultado = pagina.findChild<QToolButton*>(
        QStringLiteral("resumoResultadoOperacaoArvore"));
    auto* detalhesResultado = pagina.findChild<QLabel*>(
        QStringLiteral("detalhesResultadoOperacaoArvore"));
    auto* rolagemResultado = pagina.findChild<QScrollArea*>(
        QStringLiteral("rolagemResultadoOperacaoArvore"));
    auto* chaveBusca = pagina.findChild<QLineEdit*>(
        QStringLiteral("chaveBuscaArvore"));
    auto* buscar = pagina.findChild<QPushButton*>(
        QStringLiteral("buscarNaArvore"));
    auto* remover = pagina.findChild<QPushButton*>(
        QStringLiteral("removerDaArvore"));
    auto* metricasPeriodicas = pagina.findChild<ResultadoOperacaoArvore*>(
        QStringLiteral("metricasPeriodicasArvore"));
    auto* resumoMetricasPeriodicas = pagina.findChild<QToolButton*>(
        QStringLiteral("resumoMetricasPeriodicasArvore"));
    auto* detalhesMetricasPeriodicas = pagina.findChild<QLabel*>(
        QStringLiteral("detalhesMetricasPeriodicasArvore"));
    auto* rolagemMetricasPeriodicas = pagina.findChild<QScrollArea*>(
        QStringLiteral("rolagemMetricasPeriodicasArvore"));

    if (pagina.objectName() != QStringLiteral("paginaArvore") || !menu || !itemBuscaBinaria
        || !itemAvl || !itemRubroNegra || !itemAvlPassoAPasso
        || !conteudo || !cabecalho
        || !titulo || !subtitulo || !painelVisualizacao || !visualizacao
        || !mapa || !aumentarZoom || !percentualZoom || !modoArrasto
        || !modoFoco || !recolherSelecao || !expandirTudo
        || !percorrerEmOrdem || !percorrerPreOrdem || !percorrerPosOrdem
        || !popover || !fecharPopover || !controleExecucao || !proximoPasso
        || !resultadoOperacao || !resumoResultado || !detalhesResultado
        || !rolagemResultado
        || !chaveBusca || !buscar || !remover || !metricasPeriodicas
        || !resumoMetricasPeriodicas || !detalhesMetricasPeriodicas
        || !rolagemMetricasPeriodicas) {
        return false;
    }
    int percursosEmOrdem = 0;
    int percursosPreOrdem = 0;
    int percursosPosOrdem = 0;
    QObject::connect(&pagina, &PaginaArvore::percursoEmOrdemSolicitado,
                     &pagina, [&] { ++percursosEmOrdem; });
    QObject::connect(&pagina, &PaginaArvore::percursoPreOrdemSolicitado,
                     &pagina, [&] { ++percursosPreOrdem; });
    QObject::connect(&pagina, &PaginaArvore::percursoPosOrdemSolicitado,
                     &pagina, [&] { ++percursosPosOrdem; });
    percorrerEmOrdem->click();
    percorrerPreOrdem->click();
    percorrerPosOrdem->click();
    if (percursosEmOrdem != 1 || percursosPreOrdem != 1
        || percursosPosOrdem != 1) {
        return false;
    }
    std::optional<apresentacao::arvore::IdentificadorRegistro> chaveSolicitada;
    QObject::connect(&pagina, &PaginaArvore::buscaSolicitada, &pagina,
                     [&](auto identificador) {
                         chaveSolicitada = identificador;
                     });
    chaveBusca->setText(QStringLiteral("3,27"));
    buscar->click();
    if (chaveSolicitada
        != apresentacao::arvore::IdentificadorRegistro{3, 27}) {
        return false;
    }
    std::optional<apresentacao::arvore::IdentificadorRegistro> chaveRemovida;
    QObject::connect(&pagina, &PaginaArvore::remocaoSolicitada, &pagina,
                     [&](auto identificador) {
                         chaveRemovida = identificador;
                     });
    remover->click();
    if (chaveRemovida
        != apresentacao::arvore::IdentificadorRegistro{3, 27}) {
        return false;
    }
    chaveSolicitada.reset();
    chaveBusca->setText(QStringLiteral("3;27"));
    buscar->click();
    if (chaveSolicitada.has_value()
        || !resumoResultado->text().contains(
            QStringLiteral("formato frequência,número"))) {
        return false;
    }
    pagina.apresentarResultadoPercurso(
        QStringLiteral("Percurso em pré-ordem"),
        {{1, 20}, {1, 10}, {1, 30}}, 3, 1250);
    if (!resumoResultado->text().contains(QStringLiteral("Nós visitados: 3"))
        || !resumoResultado->text().contains(QStringLiteral("1.25 us"))
        || resultadoOperacao->expandido() || !rolagemResultado->isHidden()) {
        return false;
    }
    resumoResultado->click();
    if (!resultadoOperacao->expandido() || rolagemResultado->isHidden()
        || !detalhesResultado->wordWrap()
        || rolagemResultado->horizontalScrollBarPolicy()
            != Qt::ScrollBarAlwaysOff
        || rolagemResultado->verticalScrollBarPolicy()
            != Qt::ScrollBarAsNeeded
        || rolagemResultado->maximumHeight() > 280
        || !detalhesResultado->text().contains(
            QStringLiteral("(1, 20) → (1, 10) → (1, 30)"))) {
        return false;
    }
    if (titulo->text() != QStringLiteral("Árvore de busca binária")
        || subtitulo->text() != QStringLiteral("Estrutura final • inserções na ordem do arquivo JSON")) {
        return false;
    }
    if (!menu->testAttribute(Qt::WA_StyledBackground) || !menu->styleSheet().isEmpty()
        || !painelVisualizacao->testAttribute(Qt::WA_StyledBackground)
        || !painelVisualizacao->styleSheet().isEmpty()) {
        return false;
    }
    if (menu->minimumWidth() != 220 || menu->maximumWidth() != 320
        || painelVisualizacao->minimumWidth() != 400 || painelVisualizacao->minimumHeight() != 300
        || menu->focusPolicy() != Qt::NoFocus || painelVisualizacao->focusPolicy() != Qt::NoFocus) {
        return false;
    }
    if (!pagina.findChild<QWidget*>(QStringLiteral("metricasArvore"))
        || !pagina.findChild<QWidget*>(QStringLiteral("visualizacaoArvore"))) {
        return false;
    }
    if (menu->tipoSelecionado() != TipoEstrutura::BuscaBinaria || !itemBuscaBinaria->estaSelecionado()
        || itemAvl->estaSelecionado() || itemRubroNegra->estaSelecionado()
        || itemAvlPassoAPasso->estaSelecionado()
        || itemBuscaBinaria->focusPolicy() != Qt::StrongFocus
        || itemAvl->focusPolicy() != Qt::StrongFocus
        || itemRubroNegra->focusPolicy() != Qt::StrongFocus
        || itemAvlPassoAPasso->focusPolicy() != Qt::StrongFocus) return false;

    const apresentacao::arvore::IdentificadorRegistro raiz{2, 30};
    const apresentacao::arvore::IdentificadorRegistro esquerdo{1, 10};
    const apresentacao::arvore::IdentificadorRegistro direito{3, 40};
    pagina.apresentar({
        raiz,
        {{raiz, apresentacao::arvore::SemanticaVisualNo::RubroNegroPreto},
         {esquerdo, apresentacao::arvore::SemanticaVisualNo::RubroNegroVermelho},
         {direito}},
        {
            {raiz, esquerdo, apresentacao::arvore::LadoLigacaoArvore::Esquerdo},
            {raiz, direito, apresentacao::arvore::LadoLigacaoArvore::Direito},
        },
        {3, 2, 4, 0, 1'500'000},
    });
    QApplication::processEvents();
    const auto* textoMetricas = pagina.findChild<QLabel*>(
        QStringLiteral("metricasArvore"));
    if (!textoMetricas
        || !textoMetricas->text().contains(
            QStringLiteral("Tempo de inserção: 1.50 ms"))
        || !textoMetricas->text().contains(
            QStringLiteral("Balanceamentos (rotações): 0"))) {
        return false;
    }
    if (resumoMetricasPeriodicas->text()
        != QStringLiteral("Não foi possível calcular as métricas periódicas")) {
        return false;
    }
    pagina.apresentar({
        raiz,
        {{raiz, apresentacao::arvore::SemanticaVisualNo::RubroNegroPreto},
         {esquerdo, apresentacao::arvore::SemanticaVisualNo::RubroNegroVermelho},
         {direito}},
        {{raiz, esquerdo, apresentacao::arvore::LadoLigacaoArvore::Esquerdo},
         {raiz, direito, apresentacao::arvore::LadoLigacaoArvore::Direito}},
        {3, 2, 4, 0, 1'500'000, true, {{800, 12}, {1600, 15}}},
    });
    if (metricasPeriodicas->expandido()
        || !rolagemMetricasPeriodicas->isHidden()
        || !resumoMetricasPeriodicas->text().contains(
            QStringLiteral("2 amostras"))) {
        return false;
    }
    resumoMetricasPeriodicas->click();
    if (!metricasPeriodicas->expandido()
        || rolagemMetricasPeriodicas->isHidden()
        || detalhesMetricasPeriodicas->text()
            != QStringLiteral("• 800 inserções — altura 12\n"
                              "• 1600 inserções — altura 15")) {
        return false;
    }
    if (painelVisualizacao->layout()->indexOf(visualizacao)
            >= painelVisualizacao->layout()->indexOf(metricasPeriodicas)
        || painelVisualizacao->layout()->indexOf(metricasPeriodicas)
            >= painelVisualizacao->layout()->indexOf(resultadoOperacao)) {
        return false;
    }
    if (visualizacao->quantidadeNos() != 3 || visualizacao->zoomAtual() != 1.0) return false;
    pagina.apresentarResultadoBusca(esquerdo, true, 2, 3, 900);
    if (visualizacao->identificadorSelecionado() != esquerdo
        || !resumoResultado->text().contains(QStringLiteral("Encontrada"))
        || !resumoResultado->text().contains(QStringLiteral("Nós visitados: 2"))
        || !resumoResultado->text().contains(QStringLiteral("Comparações: 3"))
        || !resumoResultado->text().contains(QStringLiteral("900 ns"))) {
        return false;
    }
    pagina.apresentarResultadoRemocao(esquerdo, true, 3, 4, 1200);
    if (!resumoResultado->text().contains(QStringLiteral("Removida"))
        || !resumoResultado->text().contains(QStringLiteral("Nós visitados: 3"))
        || !resumoResultado->text().contains(QStringLiteral("Comparações: 4"))
        || !resumoResultado->text().contains(QStringLiteral("1.20 us"))) {
        return false;
    }
    if (visualizacao->diametroNo() < 70 || visualizacao->tamanhoFonteNumeroNo() < 14
        || visualizacao->tamanhoFonteFrequenciaNo() < 9
        || !visualizacao->corFundoNo().isValid()
        || !visualizacao->corFundoNoVermelho().isValid()
        || visualizacao->corFundoNoVermelho() == visualizacao->corFundoNo()
        || !visualizacao->corTextoNoVermelho().isValid()
        || !visualizacao->corFundoNoVermelhoSelecionado().isValid()) return false;
    ItemNoArvore* itemPreto = nullptr;
    ItemNoArvore* itemVermelho = nullptr;
    for (QGraphicsItem* itemCena : visualizacao->scene()->items()) {
        auto* itemNo = dynamic_cast<ItemNoArvore*>(itemCena);
        if (!itemNo) continue;
        if (itemNo->identificador() == raiz) itemPreto = itemNo;
        if (itemNo->identificador() == esquerdo) itemVermelho = itemNo;
    }
    if (!itemPreto || !itemVermelho
        || itemPreto->semanticaVisual()
            != apresentacao::arvore::SemanticaVisualNo::RubroNegroPreto
        || itemVermelho->semanticaVisual()
            != apresentacao::arvore::SemanticaVisualNo::RubroNegroVermelho
        || !itemPreto->toolTip().contains(QStringLiteral("preto"))
        || !itemVermelho->toolTip().contains(QStringLiteral("vermelho"))) {
        return false;
    }
    if (mapa->scene() != visualizacao->scene() || mapa->focusPolicy() != Qt::StrongFocus) return false;
    mapa->ensurePolished();
    if (mapa->minimumWidth() < 260 || mapa->minimumHeight() < 150) return false;
    if (mapa->cursor().shape() != Qt::OpenHandCursor
        || !mapa->accessibleDescription().contains(QStringLiteral("arraste"))) return false;
    if (!mapa->corAreaVisivel().isValid() || !modoArrasto->isChecked()) return false;
    if (!visualizacao->selecionarRegistro(esquerdo)
        || visualizacao->identificadorSelecionado() != esquerdo
        || visualizacao->selecionarRegistro({99, 60})) return false;

    aumentarZoom->click();
    if (visualizacao->zoomAtual() <= 1.0
        || percentualZoom->text() != QStringLiteral("120%")) {
        return false;
    }
    visualizacao->definirZoom(0.01);
    if (visualizacao->zoomAtual() != visualizacao->zoomMinimo()) return false;
    visualizacao->definirZoom(99.0);
    if (visualizacao->zoomAtual() != visualizacao->zoomMaximo()) return false;

    std::optional<apresentacao::arvore::IdentificadorRegistro> ativado;
    std::optional<apresentacao::arvore::IdentificadorRegistro> solicitado;
    QObject::connect(
        visualizacao, &VisualizacaoArvore::registroAtivado, visualizacao,
        [&ativado](apresentacao::arvore::IdentificadorRegistro identificador) {
            ativado = identificador;
        });
    QObject::connect(
        &pagina, &PaginaArvore::detalhesRegistroSolicitados, &pagina,
        [&solicitado](apresentacao::arvore::IdentificadorRegistro identificador) {
            solicitado = identificador;
        });
    QKeyEvent ativarNo(QEvent::KeyRelease, Qt::Key_Return, Qt::NoModifier);
    QApplication::sendEvent(visualizacao->scene(), &ativarNo);
    if (ativado != esquerdo || solicitado != esquerdo) {
        return false;
    }

    pagina.show();
    QApplication::processEvents();
    pagina.apresentarDetalhes({esquerdo, 2875, QStringLiteral("31/12/2025"),
                              {10, 12, 23, 34, 45, 56}});
    QApplication::processEvents();
    const auto* campoDezena = pagina.findChild<CampoDetalhe*>(
        QStringLiteral("campoDezenaDetalhes"));
    const auto* campoFrequencia = pagina.findChild<CampoDetalhe*>(
        QStringLiteral("campoFrequenciaDetalhes"));
    const auto marcadores = pagina.findChildren<MarcadorDezena*>();
    int correspondentes = 0;
    for (const auto* marcador : marcadores) {
        if (marcador->correspondente()) ++correspondentes;
    }
    const auto* painel = pagina.findChild<QWidget*>(
        QStringLiteral("painelVisualizacaoArvore"));
    if (!campoDezena || !campoFrequencia || popover->isHidden()
        || campoDezena->valor() != QStringLiteral("10")
        || campoFrequencia->valor() != QStringLiteral("1 vezes")
        || marcadores.size() != 6 || correspondentes != 1
        || !painel->rect().contains(popover->geometry())) {
        return false;
    }
    visualizacao->aumentarZoom();
    QApplication::processEvents();
    if (!painel->rect().contains(popover->geometry())) return false;
    QKeyEvent fecharDetalhes(QEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier);
    QApplication::sendEvent(fecharPopover, &fecharDetalhes);
    if (!popover->isHidden() || visualizacao->identificadorSelecionado() != esquerdo
        || !visualizacao->hasFocus()) {
        return false;
    }

    pagina.apresentarDetalhes({raiz, 2876, QStringLiteral("03/01/2026"),
                              {2, 8, 16, 30, 44, 59}});
    pagina.apresentar({raiz, {{raiz}}, {}, {1, 1, 0, 0}});
    if (!popover->isHidden()) return false;

    int quantidadeAlteracoes = 0;
    int quantidadeAlteracoesPagina = 0;
    TipoEstrutura ultimaSelecao = TipoEstrutura::BuscaBinaria;
    TipoEstrutura ultimaSelecaoPagina = TipoEstrutura::BuscaBinaria;
    QObject::connect(menu, &MenuEstruturas::estruturaSelecionada, menu,
                     [&](TipoEstrutura tipo) {
                         ++quantidadeAlteracoes;
                         ultimaSelecao = tipo;
                     });
    QObject::connect(&pagina, &PaginaArvore::estruturaSelecionada, &pagina,
                     [&](TipoEstrutura tipo) {
                         ++quantidadeAlteracoesPagina;
                         ultimaSelecaoPagina = tipo;
                     });
    itemAvl->setFocus();
    QKeyEvent pressionarEspaco(QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier);
    QKeyEvent soltarEspaco(QEvent::KeyRelease, Qt::Key_Space, Qt::NoModifier);
    QApplication::sendEvent(itemAvl, &pressionarEspaco);
    QApplication::sendEvent(itemAvl, &soltarEspaco);
    if (quantidadeAlteracoes != 1 || ultimaSelecao != TipoEstrutura::Avl
        || menu->tipoSelecionado() != TipoEstrutura::Avl || itemBuscaBinaria->estaSelecionado()
        || !itemAvl->estaSelecionado() || itemAvl->accessibleDescription() != QStringLiteral("Selecionado")
        || titulo->text() != QStringLiteral("Árvore AVL")) {
        return false;
    }

    pagina.apresentarDetalhes({raiz, 2876, QStringLiteral("03/01/2026"),
                              {2, 8, 16, 30, 44, 59}});
    itemRubroNegra->click();
    if (quantidadeAlteracoes != 2 || quantidadeAlteracoesPagina != 2
        || ultimaSelecao != TipoEstrutura::RubroNegra
        || ultimaSelecaoPagina != TipoEstrutura::RubroNegra
        || menu->tipoSelecionado() != TipoEstrutura::RubroNegra
        || itemBuscaBinaria->estaSelecionado() || itemAvl->estaSelecionado()
        || !itemRubroNegra->estaSelecionado()
        || itemAvlPassoAPasso->estaSelecionado()
        || itemRubroNegra->accessibleName() != QStringLiteral("Árvore rubro-negra")
        || titulo->text() != QStringLiteral("Árvore rubro-negra")
        || subtitulo->text() != QStringLiteral("Estrutura final • inserções na ordem do arquivo JSON")
        || !popover->isHidden()) {
        return false;
    }
    itemRubroNegra->click();
    if (quantidadeAlteracoes != 2 || quantidadeAlteracoesPagina != 2) {
        return false;
    }

    int passosSolicitados = 0;
    QObject::connect(&pagina, &PaginaArvore::proximoPassoAvlSolicitado,
                     &pagina, [&] { ++passosSolicitados; });
    itemAvlPassoAPasso->click();
    pagina.definirModoAvlPassoAPasso(true);
    pagina.atualizarExecucaoAvl(QStringLiteral("Rotação à esquerda"),
                                2, 10, 7, false);
    proximoPasso->click();
    const auto* descricaoPasso = pagina.findChild<QLabel*>(
        QStringLiteral("descricaoPassoAvl"));
    if (quantidadeAlteracoes != 3 || quantidadeAlteracoesPagina != 3
        || ultimaSelecao != TipoEstrutura::AvlPassoAPasso
        || menu->tipoSelecionado() != TipoEstrutura::AvlPassoAPasso
        || !itemAvlPassoAPasso->estaSelecionado() || itemAvl->estaSelecionado()
        || titulo->text() != QStringLiteral("AVL passo a passo")
        || subtitulo->text() != QStringLiteral("Construção interativa • uma transformação por vez")
        || controleExecucao->isHidden() || passosSolicitados != 1
        || !descricaoPasso || descricaoPasso->text() != QStringLiteral("Rotação à esquerda")) {
        return false;
    }

    itemBuscaBinaria->click();
    pagina.definirModoAvlPassoAPasso(false);
    if (quantidadeAlteracoes != 4 || quantidadeAlteracoesPagina != 4
        || ultimaSelecao != TipoEstrutura::BuscaBinaria
        || menu->tipoSelecionado() != TipoEstrutura::BuscaBinaria || !itemBuscaBinaria->estaSelecionado()
        || itemAvl->estaSelecionado() || itemRubroNegra->estaSelecionado()
        || itemAvlPassoAPasso->estaSelecionado()
        || !controleExecucao->isHidden()) {
        return false;
    }

    const QString folhaEstilos = qApp->styleSheet();
    for (const QWidget* componente : pagina.findChildren<QWidget*>()) {
        if (componente->objectName().isEmpty()
            || componente->objectName().startsWith(QStringLiteral("qt_"))) continue;
        if (!componente->styleSheet().isEmpty()
            || !folhaEstilos.contains(QStringLiteral("#") + componente->objectName())) {
            return false;
        }
    }
    return true;
}

bool testarAmbiente()
{
    Ambiente ambiente;
    PaginaTeste pagina;
    ambiente.areaPaginas().adicionarPagina(pagina);
    ambiente.areaPaginas().exibirPagina(pagina);
    ambiente.barraEstado().definirMensagem(QStringLiteral("Teste concluido."));
    return ambiente.objectName() == QStringLiteral("ambiente") && ambiente.minimumWidth() == 960
        && ambiente.minimumHeight() == 640
        && ambiente.barraEstado().mensagem() == QStringLiteral("Teste concluido.");
}

} // namespace

int main(int quantidadeArgumentos, char* argumentos[])
{
    QApplication aplicacao(quantidadeArgumentos, argumentos);
    EstiloAplicacao::aplicar(aplicacao);
    if (aplicacao.styleSheet().isEmpty() || aplicacao.styleSheet().contains(QStringLiteral("palette("))
        || aplicacao.styleSheet().contains(QStringLiteral("QWidget {"))
        || aplicacao.styleSheet().contains(QStringLiteral("* {"))) return 1;

    const QStringList seletoresObrigatorios{
        QStringLiteral("#ambiente"), QStringLiteral("#estruturaAmbiente"), QStringLiteral("#areaPaginas"),
        QStringLiteral("#paginaArvore"), QStringLiteral("#menuEstruturas"),
        QStringLiteral("#tituloMenuEstruturas"), QStringLiteral("#itemBuscaBinaria"),
        QStringLiteral("#itemAvl"),
        QStringLiteral("#itemAvlPassoAPasso"),
        QStringLiteral("#conteudoPaginaArvore"), QStringLiteral("#cabecalhoPaginaArvore"),
        QStringLiteral("#tituloPaginaArvore"), QStringLiteral("#subtituloPaginaArvore"),
        QStringLiteral("#painelVisualizacaoArvore"), QStringLiteral("#metricasArvore"),
        QStringLiteral("#metricasPeriodicasArvore"),
        QStringLiteral("#resumoMetricasPeriodicasArvore"),
        QStringLiteral("#detalhesMetricasPeriodicasArvore"),
        QStringLiteral("#rolagemMetricasPeriodicasArvore"),
        QStringLiteral("#resultadoOperacaoArvore"),
        QStringLiteral("#resumoResultadoOperacaoArvore"),
        QStringLiteral("#detalhesResultadoOperacaoArvore"),
        QStringLiteral("#rolagemResultadoOperacaoArvore"),
        QStringLiteral("#barraFerramentasArvore"), QStringLiteral("#controleZoomArvore"),
        QStringLiteral("#reduzirZoomArvore"), QStringLiteral("#percentualZoomArvore"),
        QStringLiteral("#aumentarZoomArvore"), QStringLiteral("#enquadrarArvore"),
        QStringLiteral("#modoArrastoArvore"), QStringLiteral("#modoFocoArvore"),
        QStringLiteral("#recolherSelecaoArvore"), QStringLiteral("#expandirTodaArvore"),
        QStringLiteral("#percorrerEmOrdemArvore"),
        QStringLiteral("#percorrerPreOrdemArvore"),
        QStringLiteral("#percorrerPosOrdemArvore"),
        QStringLiteral("#chaveBuscaArvore"),
        QStringLiteral("#buscarNaArvore"),
        QStringLiteral("#removerDaArvore"),
        QStringLiteral("#visualizacaoArvore"),
        QStringLiteral("#controleExecucaoAvl"), QStringLiteral("#proximoPassoAvl"),
        QStringLiteral("#reproduzirAvl"), QStringLiteral("#pausarAvl"),
        QStringLiteral("#reiniciarAvl"), QStringLiteral("#progressoPassoAvl"),
        QStringLiteral("#descricaoPassoAvl"),
        QStringLiteral("#mapaArvore"), QStringLiteral("#barraEstado"),
        QStringLiteral("#mensagemEstado"),
    };
    for (const QString& seletor : seletoresObrigatorios) {
        if (!aplicacao.styleSheet().contains(seletor)) return 1;
    }

    if (!testarComponentesBasicos()) return 2;
    if (!testarContainers()) return 3;
    if (!testarContratoVisualObrigatorio()) return 4;
    if (!testarPaginaArvore()) return 5;
    if (!testarAmbiente()) return 6;
    return 0;
}
