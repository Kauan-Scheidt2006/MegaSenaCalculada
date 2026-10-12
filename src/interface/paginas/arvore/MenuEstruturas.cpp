#include "interface/paginas/arvore/MenuEstruturas.hpp"

#include "interface/componentes/basicos/Texto.hpp"
#include "interface/paginas/arvore/ItemNavegacaoEstrutura.hpp"

#include <QSizePolicy>

#include <stdexcept>

MenuEstruturas::MenuEstruturas(QWidget* pai)
    : BaseContainer({.componente = {.nomeObjeto = QStringLiteral("menuEstruturas"),
                                    .nomeAcessivel = QStringLiteral("Estruturas"),
                                    .descricaoAcessivel = QStringLiteral("Selecione a estrutura de arvore a explorar."),
                                    .tamanhoMinimo = QSize(220, 0),
                                    .tamanhoMaximo = QSize(320, QWIDGETSIZE_MAX),
                                    .politicaTamanho = QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding),
                                    .fundoEstilizado = true},
                     .disposicao = TipoDisposicao::Vertical,
                     .margens = {16, 24, 16, 24},
                     .espacamento = 8}, pai),
      titulo_(new Texto(QStringLiteral("Estruturas"),
                        {.nomeObjeto = QStringLiteral("tituloMenuEstruturas"),
                         .fundoEstilizado = false}, this)),
      itemBuscaBinaria_(new ItemNavegacaoEstrutura(
          TipoEstrutura::BuscaBinaria, QStringLiteral("Busca binária"),
          {.nomeObjeto = QStringLiteral("itemBuscaBinaria"),
           .nomeAcessivel = QStringLiteral("Árvore de busca binária"),
           .fundoEstilizado = true}, this)),
      itemAvl_(new ItemNavegacaoEstrutura(
          TipoEstrutura::Avl, QStringLiteral("AVL"),
          {.nomeObjeto = QStringLiteral("itemAvl"),
           .nomeAcessivel = QStringLiteral("Árvore AVL"),
           .fundoEstilizado = true}, this)),
      itemRubroNegra_(new ItemNavegacaoEstrutura(
          TipoEstrutura::RubroNegra, QStringLiteral("Rubro-negra"),
          {.nomeObjeto = QStringLiteral("itemRubroNegra"),
           .nomeAcessivel = QStringLiteral("Árvore rubro-negra"),
           .fundoEstilizado = true}, this)),
      itemAvlPassoAPasso_(new ItemNavegacaoEstrutura(
          TipoEstrutura::AvlPassoAPasso, QStringLiteral("AVL passo a passo"),
          {.nomeObjeto = QStringLiteral("itemAvlPassoAPasso"),
           .nomeAcessivel = QStringLiteral("Árvore AVL passo a passo"),
           .fundoEstilizado = true}, this))
{
    setFocusPolicy(Qt::NoFocus);
    titulo_->setFocusPolicy(Qt::NoFocus);
    adicionarComponente(*titulo_);
    adicionarComponente(*itemBuscaBinaria_);
    adicionarComponente(*itemAvl_);
    adicionarComponente(*itemRubroNegra_);
    adicionarComponente(*itemAvlPassoAPasso_);
    adicionarEspacamentoExpansivel(1);

    connect(itemBuscaBinaria_, &ItemNavegacaoEstrutura::clicked, this,
            [this] { tratarAcionamento(*itemBuscaBinaria_); });
    connect(itemAvl_, &ItemNavegacaoEstrutura::clicked, this,
            [this] { tratarAcionamento(*itemAvl_); });
    connect(itemRubroNegra_, &ItemNavegacaoEstrutura::clicked, this,
            [this] { tratarAcionamento(*itemRubroNegra_); });
    connect(itemAvlPassoAPasso_, &ItemNavegacaoEstrutura::clicked, this,
            [this] { tratarAcionamento(*itemAvlPassoAPasso_); });

    itemBuscaBinaria_->definirSelecionado(true);
    itemAvl_->definirSelecionado(false);
    itemRubroNegra_->definirSelecionado(false);
    itemAvlPassoAPasso_->definirSelecionado(false);
    setTabOrder(itemBuscaBinaria_, itemAvl_);
    setTabOrder(itemAvl_, itemRubroNegra_);
    setTabOrder(itemRubroNegra_, itemAvlPassoAPasso_);
}

TipoEstrutura MenuEstruturas::tipoSelecionado() const noexcept
{
    return tipoSelecionado_;
}

void MenuEstruturas::selecionar(TipoEstrutura tipo)
{
    ItemNavegacaoEstrutura* item = nullptr;
    switch (tipo) {
    case TipoEstrutura::BuscaBinaria:
        item = itemBuscaBinaria_;
        break;
    case TipoEstrutura::Avl:
        item = itemAvl_;
        break;
    case TipoEstrutura::RubroNegra:
        item = itemRubroNegra_;
        break;
    case TipoEstrutura::AvlPassoAPasso:
        item = itemAvlPassoAPasso_;
        break;
    default:
        throw std::invalid_argument("Tipo de estrutura desconhecido.");
    }
    tratarAcionamento(*item);
}

void MenuEstruturas::tratarAcionamento(ItemNavegacaoEstrutura& item)
{
    const TipoEstrutura novoTipo = item.tipo();
    itemBuscaBinaria_->definirSelecionado(novoTipo == TipoEstrutura::BuscaBinaria);
    itemAvl_->definirSelecionado(novoTipo == TipoEstrutura::Avl);
    itemRubroNegra_->definirSelecionado(
        novoTipo == TipoEstrutura::RubroNegra);
    itemAvlPassoAPasso_->definirSelecionado(
        novoTipo == TipoEstrutura::AvlPassoAPasso);

    if (tipoSelecionado_ == novoTipo) {
        return;
    }
    tipoSelecionado_ = novoTipo;
    emit estruturaSelecionada(tipoSelecionado_);
}
