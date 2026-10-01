#include "interface/ambiente/BarraEstado.hpp"

#include "interface/componentes/basicos/Texto.hpp"

BarraEstado::BarraEstado(QWidget* pai)
    : Barra({.componente = {.nome = QStringLiteral("barraEstado"),
                            .nomeAcessivel = QStringLiteral("Estado da aplicacao")},
             .disposicao = TipoDisposicao::Horizontal,
             .margens = {16, 8, 16, 8},
             .espacamento = 8}, pai),
      mensagem_(new Texto(QStringLiteral("Interface iniciada."),
                          {.nome = QStringLiteral("mensagemEstado")}, this))
{
    adicionarComponente(*mensagem_, 1);
}

void BarraEstado::definirMensagem(const QString& mensagem)
{
    mensagem_->definirConteudo(mensagem);
}

QString BarraEstado::mensagem() const
{
    return mensagem_->text();
}
