#include "interface/paginas/arvore/CabecalhoPaginaArvore.hpp"

#include "interface/componentes/basicos/Subtitulo.hpp"
#include "interface/componentes/basicos/Titulo.hpp"

#include <QSizePolicy>

CabecalhoPaginaArvore::CabecalhoPaginaArvore(QWidget* pai)
    : BaseContainer({.componente = {.nomeObjeto = QStringLiteral("cabecalhoPaginaArvore"),
                                    .politicaTamanho = QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum),
                                    .fundoEstilizado = false},
                     .disposicao = TipoDisposicao::Vertical,
                     .espacamento = 4}, pai),
      titulo_(new Titulo(QStringLiteral("Árvore de busca binária"),
                         {.nomeObjeto = QStringLiteral("tituloPaginaArvore"),
                          .fundoEstilizado = false}, this)),
      subtitulo_(new Subtitulo(QStringLiteral("Estrutura final • inserções na ordem histórica"),
                               {.nomeObjeto = QStringLiteral("subtituloPaginaArvore"),
                                .fundoEstilizado = false}, this))
{
    adicionarComponente(*titulo_);
    adicionarComponente(*subtitulo_);
}
