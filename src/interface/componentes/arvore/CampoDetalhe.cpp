#include "interface/componentes/arvore/CampoDetalhe.hpp"

#include "interface/componentes/basicos/Texto.hpp"

CampoDetalhe::CampoDetalhe(const QString& nomeObjeto, const QString& rotulo,
                           QWidget* pai)
    : BaseContainer({.componente = {.nomeObjeto = nomeObjeto,
                                    .fundoEstilizado = true},
                     .disposicao = TipoDisposicao::Vertical,
                     .espacamento = 2}, pai),
      rotulo_(new Texto(rotulo,
                        {.nomeObjeto = nomeObjeto + QStringLiteral("Rotulo"),
                         .fundoEstilizado = false}, this)),
      valor_(new Texto(QStringLiteral("—"),
                       {.nomeObjeto = nomeObjeto + QStringLiteral("Valor"),
                        .fundoEstilizado = false}, this))
{
    adicionarComponente(*rotulo_);
    adicionarComponente(*valor_);
}

void CampoDetalhe::definirValor(const QString& valor)
{
    valor_->definirConteudo(valor);
}

QString CampoDetalhe::valor() const
{
    return valor_->text();
}
