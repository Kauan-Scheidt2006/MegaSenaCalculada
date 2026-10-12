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
      subtitulo_(new Subtitulo(QStringLiteral("Estrutura final • inserções na ordem do arquivo JSON"),
                               {.nomeObjeto = QStringLiteral("subtituloPaginaArvore"),
                                .fundoEstilizado = false}, this))
{
    adicionarComponente(*titulo_);
    adicionarComponente(*subtitulo_);
}

void CabecalhoPaginaArvore::definirTipoEstrutura(TipoEstrutura tipo)
{
    switch (tipo) {
    case TipoEstrutura::BuscaBinaria:
        titulo_->definirConteudo(QStringLiteral("Árvore de busca binária"));
        subtitulo_->definirConteudo(
            QStringLiteral("Estrutura final • inserções na ordem do arquivo JSON"));
        break;
    case TipoEstrutura::Avl:
        titulo_->definirConteudo(QStringLiteral("Árvore AVL"));
        subtitulo_->definirConteudo(
            QStringLiteral("Estrutura final • inserções na ordem do arquivo JSON"));
        break;
    case TipoEstrutura::RubroNegra:
        titulo_->definirConteudo(QStringLiteral("Árvore rubro-negra"));
        subtitulo_->definirConteudo(
            QStringLiteral("Estrutura final • inserções na ordem do arquivo JSON"));
        break;
    case TipoEstrutura::AvlPassoAPasso:
        titulo_->definirConteudo(QStringLiteral("AVL passo a passo"));
        subtitulo_->definirConteudo(
            QStringLiteral("Construção interativa • uma transformação por vez"));
        break;
    }
}
