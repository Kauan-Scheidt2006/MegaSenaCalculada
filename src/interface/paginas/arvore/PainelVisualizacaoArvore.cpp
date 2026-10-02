#include "interface/paginas/arvore/PainelVisualizacaoArvore.hpp"

#include <QSizePolicy>

PainelVisualizacaoArvore::PainelVisualizacaoArvore(QWidget* pai)
    : Painel({.componente = {.nomeObjeto = QStringLiteral("painelVisualizacaoArvore"),
                             .nomeAcessivel = QStringLiteral("Visualizacao da arvore"),
                             .descricaoAcessivel = QStringLiteral("A visualizacao ainda nao contem uma arvore."),
                             .tamanhoMinimo = QSize(400, 300),
                             .politicaTamanho = QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding),
                             .fundoEstilizado = true},
              .disposicao = TipoDisposicao::Vertical}, pai)
{
    setFocusPolicy(Qt::NoFocus);
}
