#pragma once

#include "interface/fundamentos/Painel.hpp"
#include "interface/apresentacao/arvore/DadosDetalhesRegistro.hpp"
#include "interface/apresentacao/arvore/ModeloVisualArvore.hpp"

#include <cstdint>
#include <vector>

class BarraFerramentasArvore;
class ControleExecucaoAvl;
class MapaArvore;
class PopoverDetalhesRegistro;
class ResultadoOperacaoArvore;
class Texto;
class VisualizacaoArvore;

/** Região reservada à apresentação da árvore, inicialmente sem conteúdo. */
class PainelVisualizacaoArvore final : public Painel
{
    Q_OBJECT

public:
    explicit PainelVisualizacaoArvore(QWidget* pai = nullptr);

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

protected:
    void resizeEvent(QResizeEvent* evento) override;

private:
    void reposicionarPopover();
    Texto* metricas_ = nullptr;
    ResultadoOperacaoArvore* metricasPeriodicas_ = nullptr;
    ResultadoOperacaoArvore* resultadoOperacao_ = nullptr;
    BarraFerramentasArvore* ferramentas_ = nullptr;
    ControleExecucaoAvl* controleExecucao_ = nullptr;
    VisualizacaoArvore* visualizacao_ = nullptr;
    MapaArvore* mapa_ = nullptr;
    PopoverDetalhesRegistro* popover_ = nullptr;
    std::optional<apresentacao::arvore::IdentificadorRegistro> registroDetalhado_;
};
