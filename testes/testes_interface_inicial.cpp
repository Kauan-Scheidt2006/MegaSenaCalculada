#include "interface/ambiente/Ambiente.hpp"
#include "interface/ambiente/AreaPaginas.hpp"
#include "interface/ambiente/BarraEstado.hpp"
#include "interface/componentes/basicos/BotaoNavegacao.hpp"
#include "interface/componentes/basicos/IndicadorProgresso.hpp"
#include "interface/componentes/basicos/Texto.hpp"
#include "interface/estilos/EstiloAplicacao.hpp"
#include "interface/fundamentos/BaseContainer.hpp"
#include "interface/fundamentos/BasePagina.hpp"
#include "interface/paginas/arvore/PaginaArvore.hpp"

#include <QApplication>
#include <QLabel>
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
    const auto* painelMenu = pagina.findChild<QWidget*>(QStringLiteral("painelMenuEstruturas"));
    const auto* conteudo = pagina.findChild<QWidget*>(QStringLiteral("conteudoPaginaArvore"));
    const auto* cabecalho = pagina.findChild<QWidget*>(QStringLiteral("cabecalhoPaginaArvore"));
    const auto* titulo = pagina.findChild<QLabel*>(QStringLiteral("tituloPaginaArvore"));
    const auto* subtitulo = pagina.findChild<QLabel*>(QStringLiteral("subtituloPaginaArvore"));
    const auto* painelVisualizacao = pagina.findChild<QWidget*>(QStringLiteral("painelVisualizacaoArvore"));

    if (pagina.objectName() != QStringLiteral("paginaArvore") || !painelMenu || !conteudo || !cabecalho
        || !titulo || !subtitulo || !painelVisualizacao) return false;
    if (titulo->text() != QStringLiteral("Árvore de busca binária")
        || subtitulo->text() != QStringLiteral("Estrutura final • inserções na ordem histórica")) return false;
    if (!painelMenu->testAttribute(Qt::WA_StyledBackground) || !painelMenu->styleSheet().isEmpty()
        || !painelVisualizacao->testAttribute(Qt::WA_StyledBackground)
        || !painelVisualizacao->styleSheet().isEmpty()) return false;
    if (painelMenu->minimumWidth() != 220 || painelMenu->maximumWidth() != 320
        || painelVisualizacao->minimumWidth() != 400 || painelVisualizacao->minimumHeight() != 300
        || painelMenu->focusPolicy() != Qt::NoFocus || painelVisualizacao->focusPolicy() != Qt::NoFocus) return false;
    if (!painelVisualizacao->findChildren<QWidget*>(QString{}, Qt::FindDirectChildrenOnly).isEmpty()) return false;

    const QString folhaEstilos = qApp->styleSheet();
    for (const QWidget* componente : pagina.findChildren<QWidget*>()) {
        if (componente->objectName().isEmpty() || componente->focusPolicy() != Qt::NoFocus
            || !componente->styleSheet().isEmpty()
            || !folhaEstilos.contains(QStringLiteral("#") + componente->objectName())) return false;
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
        QStringLiteral("#paginaArvore"), QStringLiteral("#painelMenuEstruturas"),
        QStringLiteral("#conteudoPaginaArvore"), QStringLiteral("#cabecalhoPaginaArvore"),
        QStringLiteral("#tituloPaginaArvore"), QStringLiteral("#subtituloPaginaArvore"),
        QStringLiteral("#painelVisualizacaoArvore"), QStringLiteral("#barraEstado"),
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
