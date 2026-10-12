#include "interface/componentes/arvore/ControleExecucaoAvl.hpp"

#include "interface/componentes/basicos/Botao.hpp"
#include "interface/componentes/basicos/Texto.hpp"

#include <QSizePolicy>

ControleExecucaoAvl::ControleExecucaoAvl(QWidget* pai)
    : Barra({.componente = {.nomeObjeto = QStringLiteral("controleExecucaoAvl"),
                            .nomeAcessivel = QStringLiteral("Execução da AVL passo a passo"),
                            .politicaTamanho = QSizePolicy(QSizePolicy::Expanding,
                                                          QSizePolicy::Maximum),
                            .fundoEstilizado = true,
                            .visivel = false},
             .disposicao = TipoDisposicao::Horizontal,
             .margens = {8, 8, 8, 8},
             .espacamento = 8}, pai),
      proximo_(new Botao(QStringLiteral("Próximo passo"),
                          {.nomeObjeto = QStringLiteral("proximoPassoAvl"),
                           .fundoEstilizado = true}, this)),
      reproduzir_(new Botao(QStringLiteral("Reproduzir"),
                            {.nomeObjeto = QStringLiteral("reproduzirAvl"),
                             .fundoEstilizado = true}, this)),
      pausar_(new Botao(QStringLiteral("Pausar"),
                        {.nomeObjeto = QStringLiteral("pausarAvl"),
                         .fundoEstilizado = true}, this)),
      reiniciar_(new Botao(QStringLiteral("Reiniciar"),
                           {.nomeObjeto = QStringLiteral("reiniciarAvl"),
                            .fundoEstilizado = true}, this)),
      progresso_(new Texto(QStringLiteral("Registro 0 de 0 • Passo 0"),
                           {.nomeObjeto = QStringLiteral("progressoPassoAvl"),
                            .fundoEstilizado = false}, this)),
      descricao_(new Texto(QStringLiteral("Árvore vazia."),
                           {.nomeObjeto = QStringLiteral("descricaoPassoAvl"),
                            .fundoEstilizado = false}, this))
{
    adicionarComponente(*proximo_);
    adicionarComponente(*reproduzir_);
    adicionarComponente(*pausar_);
    adicionarComponente(*reiniciar_);
    adicionarComponente(*progresso_);
    adicionarComponente(*descricao_, 1);
    pausar_->setEnabled(false);

    connect(proximo_, &Botao::clicked, this,
            &ControleExecucaoAvl::proximoPassoSolicitado);
    connect(reproduzir_, &Botao::clicked, this,
            &ControleExecucaoAvl::reproducaoSolicitada);
    connect(pausar_, &Botao::clicked, this,
            &ControleExecucaoAvl::pausaSolicitada);
    connect(reiniciar_, &Botao::clicked, this,
            &ControleExecucaoAvl::reinicioSolicitado);
}

void ControleExecucaoAvl::definirModoAtivo(bool ativo)
{
    setVisible(ativo);
    if (!ativo) definirExecutando(false);
}

void ControleExecucaoAvl::definirExecutando(bool executando)
{
    proximo_->setEnabled(!executando && !concluido_);
    reproduzir_->setEnabled(!executando && !concluido_);
    pausar_->setEnabled(executando);
    reiniciar_->setEnabled(true);
    setAccessibleDescription(executando
        ? QStringLiteral("Reprodução automática em andamento")
        : QStringLiteral("Reprodução parada"));
}

void ControleExecucaoAvl::definirConcluido(bool concluido)
{
    concluido_ = concluido;
    if (concluido) definirExecutando(false);
}

void ControleExecucaoAvl::definirInformacoes(
    const QString& descricao, std::size_t registroAtual,
    std::size_t totalRegistros, std::size_t numeroPasso)
{
    progresso_->definirConteudo(
        QStringLiteral("Registro %1 de %2 • Passo %3")
            .arg(registroAtual).arg(totalRegistros).arg(numeroPasso));
    descricao_->definirConteudo(descricao);
}
