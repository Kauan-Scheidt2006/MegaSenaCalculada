#include "interface/paginas/arvore/PaginaArvore.hpp"

#include "interface/paginas/arvore/ConteudoPaginaArvore.hpp"
#include "interface/paginas/arvore/MenuEstruturas.hpp"

#include <QSizePolicy>

PaginaArvore::PaginaArvore(QWidget* pai)
    : BasePagina({.componente = {.nomeObjeto = QStringLiteral("paginaArvore"),
                                 .politicaTamanho = QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding),
                                 .fundoEstilizado = false},
                  .disposicao = TipoDisposicao::Horizontal}, pai),
      menuEstruturas_(new MenuEstruturas(this)),
      conteudo_(new ConteudoPaginaArvore(this))
{
    adicionarComponente(*menuEstruturas_);
    adicionarComponente(*conteudo_, 1);

    connect(menuEstruturas_, &MenuEstruturas::estruturaSelecionada,
            this, [this](TipoEstrutura tipo) {
                conteudo_->fecharDetalhes();
                conteudo_->definirTipoEstrutura(tipo);
                emit estruturaSelecionada(tipo);
            });
    connect(conteudo_, &ConteudoPaginaArvore::detalhesRegistroSolicitados,
            this, &PaginaArvore::detalhesRegistroSolicitados);
    connect(conteudo_, &ConteudoPaginaArvore::proximoPassoAvlSolicitado,
            this, &PaginaArvore::proximoPassoAvlSolicitado);
    connect(conteudo_, &ConteudoPaginaArvore::reproducaoAvlSolicitada,
            this, &PaginaArvore::reproducaoAvlSolicitada);
    connect(conteudo_, &ConteudoPaginaArvore::pausaAvlSolicitada,
            this, &PaginaArvore::pausaAvlSolicitada);
    connect(conteudo_, &ConteudoPaginaArvore::reinicioAvlSolicitado,
            this, &PaginaArvore::reinicioAvlSolicitado);
    connect(conteudo_, &ConteudoPaginaArvore::percursoEmOrdemSolicitado,
            this, &PaginaArvore::percursoEmOrdemSolicitado);
    connect(conteudo_, &ConteudoPaginaArvore::percursoPreOrdemSolicitado,
            this, &PaginaArvore::percursoPreOrdemSolicitado);
    connect(conteudo_, &ConteudoPaginaArvore::percursoPosOrdemSolicitado,
            this, &PaginaArvore::percursoPosOrdemSolicitado);
    connect(conteudo_, &ConteudoPaginaArvore::buscaSolicitada,
            this, &PaginaArvore::buscaSolicitada);
    connect(conteudo_, &ConteudoPaginaArvore::remocaoSolicitada,
            this, &PaginaArvore::remocaoSolicitada);
}

void PaginaArvore::apresentarDetalhes(
    const apresentacao::arvore::DadosDetalhesRegistro& detalhes)
{
    conteudo_->apresentarDetalhes(detalhes);
}

void PaginaArvore::fecharDetalhes()
{
    conteudo_->fecharDetalhes();
}

void PaginaArvore::definirModoAvlPassoAPasso(bool ativo)
{
    conteudo_->definirModoAvlPassoAPasso(ativo);
}

void PaginaArvore::atualizarExecucaoAvl(
    const QString& descricao, std::size_t registroAtual,
    std::size_t totalRegistros, std::size_t numeroPasso, bool concluido)
{
    conteudo_->atualizarExecucaoAvl(
        descricao, registroAtual, totalRegistros, numeroPasso, concluido);
}

void PaginaArvore::definirReproducaoAvl(bool executando)
{
    conteudo_->definirReproducaoAvl(executando);
}

void PaginaArvore::apresentarResultadoPercurso(
    const QString& nomePercurso,
    const std::vector<apresentacao::arvore::IdentificadorRegistro>& ordem,
    std::size_t quantidadeVisitada,
    std::uint64_t tempoExecucaoNanossegundos)
{
    conteudo_->apresentarResultadoPercurso(
        nomePercurso, ordem, quantidadeVisitada,
        tempoExecucaoNanossegundos);
}

void PaginaArvore::apresentarResultadoBusca(
    apresentacao::arvore::IdentificadorRegistro identificador,
    bool encontrado, std::size_t nosVisitados,
    std::size_t comparacoes,
    std::uint64_t tempoExecucaoNanossegundos)
{
    conteudo_->apresentarResultadoBusca(
        identificador, encontrado, nosVisitados, comparacoes,
        tempoExecucaoNanossegundos);
}

void PaginaArvore::apresentarResultadoRemocao(
    apresentacao::arvore::IdentificadorRegistro identificador,
    bool removido, std::size_t nosVisitados,
    std::size_t comparacoes,
    std::uint64_t tempoExecucaoNanossegundos)
{
    conteudo_->apresentarResultadoRemocao(
        identificador, removido, nosVisitados, comparacoes,
        tempoExecucaoNanossegundos);
}

void PaginaArvore::apresentar(
    const apresentacao::arvore::ModeloVisualArvore& modelo,
    bool preservarViewport)
{
    conteudo_->apresentar(modelo, preservarViewport);
}
