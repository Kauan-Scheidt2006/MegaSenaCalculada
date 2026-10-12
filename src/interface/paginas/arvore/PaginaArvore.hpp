#pragma once

#include "interface/fundamentos/BasePagina.hpp"
#include "interface/apresentacao/arvore/ModeloVisualArvore.hpp"
#include "interface/apresentacao/arvore/DadosDetalhesRegistro.hpp"
#include "interface/paginas/arvore/TipoEstrutura.hpp"

#include <cstdint>
#include <vector>

class ConteudoPaginaArvore;
class MenuEstruturas;

/** Página inicial para exploração das estruturas de árvore. */
class PaginaArvore final : public BasePagina
{
    Q_OBJECT

public:
    explicit PaginaArvore(QWidget* pai = nullptr);

    void apresentar(const apresentacao::arvore::ModeloVisualArvore& modelo,
                    bool preservarViewport = false);
    void apresentarDetalhes(
        const apresentacao::arvore::DadosDetalhesRegistro& detalhes);
    void fecharDetalhes();
    void definirModoAvlPassoAPasso(bool ativo);
    void atualizarExecucaoAvl(const QString& descricao,
                              std::size_t registroAtual,
                              std::size_t totalRegistros,
                              std::size_t numeroPasso, bool concluido);
    void definirReproducaoAvl(bool executando);
    void apresentarResultadoPercurso(
        const QString& nomePercurso,
        const std::vector<apresentacao::arvore::IdentificadorRegistro>& ordem,
        std::size_t quantidadeVisitada,
        std::uint64_t tempoExecucaoNanossegundos);
    void apresentarResultadoBusca(
        apresentacao::arvore::IdentificadorRegistro identificador,
        bool encontrado, std::size_t nosVisitados,
        std::size_t comparacoes,
        std::uint64_t tempoExecucaoNanossegundos);
    void apresentarResultadoRemocao(
        apresentacao::arvore::IdentificadorRegistro identificador,
        bool removido, std::size_t nosVisitados,
        std::size_t comparacoes,
        std::uint64_t tempoExecucaoNanossegundos);

signals:
    void estruturaSelecionada(TipoEstrutura tipo);
    void detalhesRegistroSolicitados(
        apresentacao::arvore::IdentificadorRegistro identificador);
    void proximoPassoAvlSolicitado();
    void reproducaoAvlSolicitada();
    void pausaAvlSolicitada();
    void reinicioAvlSolicitado();
    void percursoEmOrdemSolicitado();
    void percursoPreOrdemSolicitado();
    void percursoPosOrdemSolicitado();
    void buscaSolicitada(
        apresentacao::arvore::IdentificadorRegistro identificador);
    void remocaoSolicitada(
        apresentacao::arvore::IdentificadorRegistro identificador);

private:
    MenuEstruturas* menuEstruturas_ = nullptr;
    ConteudoPaginaArvore* conteudo_ = nullptr;
};
