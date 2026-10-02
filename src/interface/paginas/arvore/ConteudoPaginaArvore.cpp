#include "interface/paginas/arvore/ConteudoPaginaArvore.hpp"

#include "interface/paginas/arvore/CabecalhoPaginaArvore.hpp"
#include "interface/paginas/arvore/PainelVisualizacaoArvore.hpp"

#include <QSizePolicy>

ConteudoPaginaArvore::ConteudoPaginaArvore(QWidget* pai)
    : BaseContainer({.componente = {.nomeObjeto = QStringLiteral("conteudoPaginaArvore"),
                                    .politicaTamanho = QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding),
                                    .fundoEstilizado = false},
                     .disposicao = TipoDisposicao::Vertical,
                     .margens = {24, 16, 24, 16},
                     .espacamento = 12}, pai),
      cabecalho_(new CabecalhoPaginaArvore(this)),
      painelVisualizacao_(new PainelVisualizacaoArvore(this))
{
    adicionarComponente(*cabecalho_);
    adicionarComponente(*painelVisualizacao_, 1);
}
