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
    connect(painelVisualizacao_,
            &PainelVisualizacaoArvore::detalhesRegistroSolicitados,
            this, &ConteudoPaginaArvore::detalhesRegistroSolicitados);
    connect(painelVisualizacao_, &PainelVisualizacaoArvore::proximoPassoAvlSolicitado,
            this, &ConteudoPaginaArvore::proximoPassoAvlSolicitado);
    connect(painelVisualizacao_, &PainelVisualizacaoArvore::reproducaoAvlSolicitada,
            this, &ConteudoPaginaArvore::reproducaoAvlSolicitada);
    connect(painelVisualizacao_, &PainelVisualizacaoArvore::pausaAvlSolicitada,
            this, &ConteudoPaginaArvore::pausaAvlSolicitada);
    connect(painelVisualizacao_, &PainelVisualizacaoArvore::reinicioAvlSolicitado,
            this, &ConteudoPaginaArvore::reinicioAvlSolicitado);
    connect(painelVisualizacao_, &PainelVisualizacaoArvore::percursoEmOrdemSolicitado,
            this, &ConteudoPaginaArvore::percursoEmOrdemSolicitado);
    connect(painelVisualizacao_, &PainelVisualizacaoArvore::percursoPreOrdemSolicitado,
            this, &ConteudoPaginaArvore::percursoPreOrdemSolicitado);
    connect(painelVisualizacao_, &PainelVisualizacaoArvore::percursoPosOrdemSolicitado,
            this, &ConteudoPaginaArvore::percursoPosOrdemSolicitado);
    connect(painelVisualizacao_, &PainelVisualizacaoArvore::buscaSolicitada,
            this, &ConteudoPaginaArvore::buscaSolicitada);
    connect(painelVisualizacao_, &PainelVisualizacaoArvore::remocaoSolicitada,
            this, &ConteudoPaginaArvore::remocaoSolicitada);
}

void ConteudoPaginaArvore::apresentar(
    const apresentacao::arvore::ModeloVisualArvore& modelo,
    bool preservarViewport)
{
    painelVisualizacao_->apresentar(modelo, preservarViewport);
}

void ConteudoPaginaArvore::definirTipoEstrutura(TipoEstrutura tipo)
{
    cabecalho_->definirTipoEstrutura(tipo);
}

void ConteudoPaginaArvore::apresentarDetalhes(
    const apresentacao::arvore::DadosDetalhesRegistro& detalhes)
{
    painelVisualizacao_->apresentarDetalhes(detalhes);
}

void ConteudoPaginaArvore::fecharDetalhes()
{
    painelVisualizacao_->fecharDetalhes();
}

void ConteudoPaginaArvore::definirModoAvlPassoAPasso(bool ativo)
{
    painelVisualizacao_->definirModoAvlPassoAPasso(ativo);
}

void ConteudoPaginaArvore::atualizarExecucaoAvl(
    const QString& descricao, std::size_t registroAtual,
    std::size_t totalRegistros, std::size_t numeroPasso, bool concluido)
{
    painelVisualizacao_->atualizarExecucaoAvl(
        descricao, registroAtual, totalRegistros, numeroPasso, concluido);
}

void ConteudoPaginaArvore::definirReproducaoAvl(bool executando)
{
    painelVisualizacao_->definirReproducaoAvl(executando);
}

void ConteudoPaginaArvore::apresentarResultadoPercurso(
    const QString& nomePercurso,
    const std::vector<apresentacao::arvore::IdentificadorRegistro>& ordem,
    std::size_t quantidadeVisitada,
    std::uint64_t tempoExecucaoNanossegundos)
{
    painelVisualizacao_->apresentarResultadoPercurso(
        nomePercurso, ordem, quantidadeVisitada,
        tempoExecucaoNanossegundos);
}

void ConteudoPaginaArvore::apresentarResultadoBusca(
    apresentacao::arvore::IdentificadorRegistro identificador,
    bool encontrado, std::size_t nosVisitados,
    std::size_t comparacoes,
    std::uint64_t tempoExecucaoNanossegundos)
{
    painelVisualizacao_->apresentarResultadoBusca(
        identificador, encontrado, nosVisitados, comparacoes,
        tempoExecucaoNanossegundos);
}

void ConteudoPaginaArvore::apresentarResultadoRemocao(
    apresentacao::arvore::IdentificadorRegistro identificador,
    bool removido, std::size_t nosVisitados,
    std::size_t comparacoes,
    std::uint64_t tempoExecucaoNanossegundos)
{
    painelVisualizacao_->apresentarResultadoRemocao(
        identificador, removido, nosVisitados, comparacoes,
        tempoExecucaoNanossegundos);
}
