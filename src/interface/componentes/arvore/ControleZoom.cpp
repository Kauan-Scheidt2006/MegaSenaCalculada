#include "interface/componentes/arvore/ControleZoom.hpp"

#include "interface/componentes/basicos/Botao.hpp"
#include "interface/componentes/basicos/Texto.hpp"

#include <QSizePolicy>

ControleZoom::ControleZoom(QWidget* pai)
    : BaseContainer(
          {.componente = {.nomeObjeto = QStringLiteral("controleZoomArvore"),
                           .nomeAcessivel = QStringLiteral("Controle de zoom"),
                           .politicaTamanho = QSizePolicy(QSizePolicy::Maximum,
                                                         QSizePolicy::Maximum),
                           .fundoEstilizado = true},
           .disposicao = TipoDisposicao::Horizontal,
           .espacamento = 0},
          pai),
      reduzir_(new Botao(
          QStringLiteral("−"),
          {.nomeObjeto = QStringLiteral("reduzirZoomArvore"),
           .nomeAcessivel = QStringLiteral("Reduzir zoom"),
           .fundoEstilizado = true}, this)),
      percentual_(new Texto(
          QStringLiteral("100%"),
          {.nomeObjeto = QStringLiteral("percentualZoomArvore"),
           .nomeAcessivel = QStringLiteral("Zoom atual: 100 por cento"),
           .fundoEstilizado = true}, this)),
      aumentar_(new Botao(
          QStringLiteral("+"),
          {.nomeObjeto = QStringLiteral("aumentarZoomArvore"),
           .nomeAcessivel = QStringLiteral("Aumentar zoom"),
           .fundoEstilizado = true}, this))
{
    adicionarComponente(*reduzir_);
    adicionarComponente(*percentual_);
    adicionarComponente(*aumentar_);
    connect(reduzir_, &Botao::clicked, this, &ControleZoom::reducaoSolicitada);
    connect(aumentar_, &Botao::clicked, this, &ControleZoom::aumentoSolicitado);
}

void ControleZoom::definirPercentual(int percentual)
{
    percentual_->definirConteudo(QStringLiteral("%1%").arg(percentual));
    percentual_->setAccessibleName(
        QStringLiteral("Zoom atual: %1 por cento").arg(percentual));
}
