#include "interface/ambiente/Ambiente.hpp"
#include "interface/ambiente/AreaPaginas.hpp"
#include "interface/ambiente/BarraEstado.hpp"
#include "interface/componentes/basicos/BotaoNavegacao.hpp"
#include "interface/componentes/basicos/IndicadorProgresso.hpp"
#include "interface/componentes/basicos/Texto.hpp"
#include "interface/fundamentos/BaseContainer.hpp"
#include "interface/fundamentos/BasePagina.hpp"

#include <QApplication>

#include <stdexcept>

namespace {

class ContainerTeste final : public BaseContainer
{
public:
    explicit ContainerTeste(TipoDisposicao disposicao)
        : BaseContainer({.componente = {.nome = QStringLiteral("containerTeste")}, .disposicao = disposicao})
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
        : BasePagina({.componente = {.nome = QStringLiteral("paginaTeste")},
                      .disposicao = TipoDisposicao::Vertical})
    {
    }
};

bool testarComponentesBasicos()
{
    Texto texto(QStringLiteral("Inicial"));
    texto.definirConteudo(QStringLiteral("Atualizado"));
    if (texto.text() != QStringLiteral("Atualizado") || texto.accessibleName() != QStringLiteral("Atualizado")) {
        return false;
    }

    BotaoNavegacao navegacao(QStringLiteral("AVL"));
    navegacao.definirSelecionado(true);
    if (!navegacao.estaSelecionado() || navegacao.focusPolicy() != Qt::StrongFocus) {
        return false;
    }

    IndicadorProgresso progresso;
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
    Texto primeiro(QStringLiteral("Primeiro"), {}, &linear);
    linear.adicionarComponente(primeiro);
    if (linear.quantidadeComponentes() != 1) {
        return false;
    }

    ContainerTeste grade(TipoDisposicao::Grade);
    Texto celula(QStringLiteral("Celula"), {}, &grade);
    grade.adicionarComponenteNaGrade(celula, 0, 0);
    if (grade.quantidadeComponentes() != 1) {
        return false;
    }

    try {
        grade.adicionarComponente(celula);
        return false;
    } catch (const std::logic_error&) {
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

    return ambiente.objectName() == QStringLiteral("ambiente")
        && ambiente.minimumWidth() == 960
        && ambiente.minimumHeight() == 640
        && ambiente.barraEstado().mensagem() == QStringLiteral("Teste concluido.");
}

} // namespace

int main(int quantidadeArgumentos, char* argumentos[])
{
    QApplication aplicacao(quantidadeArgumentos, argumentos);
    return testarComponentesBasicos() && testarContainers() && testarAmbiente() ? 0 : 1;
}
