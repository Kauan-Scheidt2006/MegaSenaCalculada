#include "interface/paginas/arvore/PaginaArvore.hpp"

#include "interface/fundamentos/Painel.hpp"
#include "interface/paginas/arvore/ConteudoPaginaArvore.hpp"

#include <QSizePolicy>

PaginaArvore::PaginaArvore(QWidget* pai)
    : BasePagina({.componente = {.nomeObjeto = QStringLiteral("paginaArvore"),
                                 .politicaTamanho = QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding),
                                 .fundoEstilizado = false},
                  .disposicao = TipoDisposicao::Horizontal}, pai),
      painelMenuEstruturas_(new Painel(
          {.componente = {.nomeObjeto = QStringLiteral("painelMenuEstruturas"),
                          .nomeAcessivel = QStringLiteral("Menu de estruturas"),
                          .descricaoAcessivel = QStringLiteral("Area reservada a navegacao entre estruturas."),
                          .tamanhoMinimo = QSize(220, 0),
                          .tamanhoMaximo = QSize(320, QWIDGETSIZE_MAX),
                          .politicaTamanho = QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding),
                          .fundoEstilizado = true},
           .disposicao = TipoDisposicao::Vertical,
           .margens = {16, 24, 16, 24}}, this)),
      conteudo_(new ConteudoPaginaArvore(this))
{
    painelMenuEstruturas_->setFocusPolicy(Qt::NoFocus);
    adicionarComponente(*painelMenuEstruturas_);
    adicionarComponente(*conteudo_, 1);
}
