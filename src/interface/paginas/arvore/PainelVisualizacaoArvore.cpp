#include "interface/paginas/arvore/PainelVisualizacaoArvore.hpp"

#include "interface/componentes/arvore/BarraFerramentasArvore.hpp"
#include "interface/componentes/arvore/ControleExecucaoAvl.hpp"
#include "interface/componentes/arvore/MapaArvore.hpp"
#include "interface/componentes/arvore/PopoverDetalhesRegistro.hpp"
#include "interface/componentes/arvore/ResultadoOperacaoArvore.hpp"
#include "interface/componentes/arvore/VisualizacaoArvore.hpp"
#include "interface/componentes/basicos/Texto.hpp"

#include <QSizePolicy>
#include <QResizeEvent>
#include <QStringList>

#include <algorithm>

namespace {

QString formatarTempoInsercao(std::uint64_t nanossegundos)
{
    if (nanossegundos < 1'000) {
        return QStringLiteral("%1 ns").arg(nanossegundos);
    }
    if (nanossegundos < 1'000'000) {
        return QStringLiteral("%1 us").arg(
            static_cast<double>(nanossegundos) / 1'000.0, 0, 'f', 2);
    }
    return QStringLiteral("%1 ms").arg(
        static_cast<double>(nanossegundos) / 1'000'000.0, 0, 'f', 2);
}

QString formatarOrdemPercurso(
    const std::vector<apresentacao::arvore::IdentificadorRegistro>& ordem)
{
    QStringList itens;
    itens.reserve(static_cast<qsizetype>(ordem.size()));
    for (const auto& identificador : ordem) {
        itens.push_back(QStringLiteral("(%1, %2)")
                            .arg(identificador.frequenciaAcumulada)
                            .arg(identificador.numero));
    }
    return itens.join(QStringLiteral(" → "));
}

QString formatarItensMetricasPeriodicas(
    const apresentacao::arvore::MetricasArvore& metricas)
{
    QStringList amostras;
    amostras.reserve(
        static_cast<qsizetype>(metricas.alturasPeriodicas.size()));
    for (const auto& amostra : metricas.alturasPeriodicas) {
        amostras.push_back(QStringLiteral("• %1 inserções — altura %2")
                               .arg(amostra.quantidadeInserida)
                               .arg(amostra.altura));
    }
    return amostras.join(QLatin1Char('\n'));
}

} // namespace

PainelVisualizacaoArvore::PainelVisualizacaoArvore(QWidget* pai)
    : Painel({.componente = {.nomeObjeto = QStringLiteral("painelVisualizacaoArvore"),
                             .nomeAcessivel = QStringLiteral("Visualizacao da arvore"),
                             .descricaoAcessivel = QStringLiteral("A visualizacao ainda nao contem uma arvore."),
                             .tamanhoMinimo = QSize(400, 300),
                             .politicaTamanho = QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding),
                             .fundoEstilizado = true},
              .disposicao = TipoDisposicao::Vertical,
              .margens = {12, 12, 12, 12},
              .espacamento = 8}, pai),
      metricas_(new Texto(QStringLiteral("Aguardando os registros do arquivo JSON."),
                          {.nomeObjeto = QStringLiteral("metricasArvore"),
                           .fundoEstilizado = false}, this)),
      metricasPeriodicas_(new ResultadoOperacaoArvore(
          QStringLiteral("metricasPeriodicasArvore"),
          QStringLiteral("resumoMetricasPeriodicasArvore"),
          QStringLiteral("detalhesMetricasPeriodicasArvore"),
          QStringLiteral("rolagemMetricasPeriodicasArvore"), this)),
      resultadoOperacao_(new ResultadoOperacaoArvore(
          QStringLiteral("resultadoOperacaoArvore"),
          QStringLiteral("resumoResultadoOperacaoArvore"),
          QStringLiteral("detalhesResultadoOperacaoArvore"),
          QStringLiteral("rolagemResultadoOperacaoArvore"), this)),
      ferramentas_(new BarraFerramentasArvore(this)),
      controleExecucao_(new ControleExecucaoAvl(this)),
      visualizacao_(new VisualizacaoArvore(this)),
      mapa_(new MapaArvore(this)),
      popover_(new PopoverDetalhesRegistro(this))
{
    adicionarComponente(*metricas_);
    adicionarComponente(*ferramentas_);
    adicionarComponente(*controleExecucao_);
    adicionarComponente(*visualizacao_, 1);
    adicionarComponente(*mapa_, 0, Qt::AlignRight);
    adicionarComponente(*metricasPeriodicas_);
    adicionarComponente(*resultadoOperacao_);
    metricasPeriodicas_->definirMensagem(
        QStringLiteral("Aguardando as métricas periódicas."));
    resultadoOperacao_->definirMensagem(
        QStringLiteral("Selecione um percurso para consultar a árvore."));

    mapa_->vincular(*visualizacao_);
    connect(ferramentas_, &BarraFerramentasArvore::aumentoZoomSolicitado,
            visualizacao_, &VisualizacaoArvore::aumentarZoom);
    connect(ferramentas_, &BarraFerramentasArvore::reducaoZoomSolicitada,
            visualizacao_, &VisualizacaoArvore::reduzirZoom);
    connect(ferramentas_, &BarraFerramentasArvore::enquadramentoSolicitado,
            visualizacao_, &VisualizacaoArvore::enquadrar);
    connect(ferramentas_, &BarraFerramentasArvore::modoArrastoAlterado,
            visualizacao_, &VisualizacaoArvore::definirModoArrasto);
    connect(ferramentas_, &BarraFerramentasArvore::modoFocoAlterado,
            visualizacao_, &VisualizacaoArvore::definirModoFoco);
    connect(ferramentas_, &BarraFerramentasArvore::expansaoTotalSolicitada,
            visualizacao_, &VisualizacaoArvore::expandirTudo);
    connect(ferramentas_, &BarraFerramentasArvore::recolhimentoSelecaoSolicitado,
            visualizacao_, &VisualizacaoArvore::recolherOuExpandirSelecao);
    connect(ferramentas_, &BarraFerramentasArvore::percursoEmOrdemSolicitado,
            this, &PainelVisualizacaoArvore::percursoEmOrdemSolicitado);
    connect(ferramentas_, &BarraFerramentasArvore::percursoPreOrdemSolicitado,
            this, &PainelVisualizacaoArvore::percursoPreOrdemSolicitado);
    connect(ferramentas_, &BarraFerramentasArvore::percursoPosOrdemSolicitado,
            this, &PainelVisualizacaoArvore::percursoPosOrdemSolicitado);
    connect(ferramentas_, &BarraFerramentasArvore::buscaSolicitada,
            this, &PainelVisualizacaoArvore::buscaSolicitada);
    connect(ferramentas_, &BarraFerramentasArvore::remocaoSolicitada,
            this, &PainelVisualizacaoArvore::remocaoSolicitada);
    connect(ferramentas_, &BarraFerramentasArvore::buscaInvalida,
            this, [this](const QString& mensagem) {
                resultadoOperacao_->definirMensagem(mensagem);
            });
    connect(visualizacao_, &VisualizacaoArvore::zoomAlterado,
            ferramentas_, &BarraFerramentasArvore::definirPercentualZoom);
    connect(visualizacao_, &VisualizacaoArvore::registroAtivado,
            this, &PainelVisualizacaoArvore::detalhesRegistroSolicitados);
    connect(visualizacao_, &VisualizacaoArvore::viewportAlterado,
            this, &PainelVisualizacaoArvore::reposicionarPopover);
    connect(popover_, &PopoverDetalhesRegistro::fechamentoSolicitado,
            this, &PainelVisualizacaoArvore::fecharDetalhes);
    connect(controleExecucao_, &ControleExecucaoAvl::proximoPassoSolicitado,
            this, &PainelVisualizacaoArvore::proximoPassoAvlSolicitado);
    connect(controleExecucao_, &ControleExecucaoAvl::reproducaoSolicitada,
            this, &PainelVisualizacaoArvore::reproducaoAvlSolicitada);
    connect(controleExecucao_, &ControleExecucaoAvl::pausaSolicitada,
            this, &PainelVisualizacaoArvore::pausaAvlSolicitada);
    connect(controleExecucao_, &ControleExecucaoAvl::reinicioSolicitado,
            this, &PainelVisualizacaoArvore::reinicioAvlSolicitado);
}

void PainelVisualizacaoArvore::apresentarResultadoPercurso(
    const QString& nomePercurso,
    const std::vector<apresentacao::arvore::IdentificadorRegistro>& ordem,
    std::size_t quantidadeVisitada,
    std::uint64_t tempoExecucaoNanossegundos)
{
    resultadoOperacao_->definirResultado(
        QStringLiteral("%1 • Nós visitados: %2 • Tempo: %3")
            .arg(nomePercurso)
            .arg(quantidadeVisitada)
            .arg(formatarTempoInsercao(tempoExecucaoNanossegundos)),
        formatarOrdemPercurso(ordem));
}

void PainelVisualizacaoArvore::apresentarResultadoBusca(
    apresentacao::arvore::IdentificadorRegistro identificador,
    bool encontrado, std::size_t nosVisitados,
    std::size_t comparacoes,
    std::uint64_t tempoExecucaoNanossegundos)
{
    if (encontrado) {
        const bool focado = visualizacao_->focarRegistro(identificador);
        Q_UNUSED(focado);
    }
    resultadoOperacao_->definirMensagem(
        QStringLiteral("Busca pela chave (%1, %2) • %3 • Nós visitados: %4 • Comparações: %5 • Tempo: %6")
            .arg(identificador.frequenciaAcumulada)
            .arg(identificador.numero)
            .arg(encontrado ? QStringLiteral("Encontrada")
                            : QStringLiteral("Não encontrada"))
            .arg(nosVisitados)
            .arg(comparacoes)
            .arg(formatarTempoInsercao(tempoExecucaoNanossegundos)));
}

void PainelVisualizacaoArvore::apresentarResultadoRemocao(
    apresentacao::arvore::IdentificadorRegistro identificador,
    bool removido, std::size_t nosVisitados,
    std::size_t comparacoes,
    std::uint64_t tempoExecucaoNanossegundos)
{
    resultadoOperacao_->definirMensagem(
        QStringLiteral("Remoção da chave (%1, %2) • %3 • Nós visitados: %4 • Comparações: %5 • Tempo: %6")
            .arg(identificador.frequenciaAcumulada)
            .arg(identificador.numero)
            .arg(removido ? QStringLiteral("Removida")
                          : QStringLiteral("Não encontrada"))
            .arg(nosVisitados)
            .arg(comparacoes)
            .arg(formatarTempoInsercao(tempoExecucaoNanossegundos)));
}

void PainelVisualizacaoArvore::definirModoAvlPassoAPasso(bool ativo)
{
    controleExecucao_->definirModoAtivo(ativo);
    ferramentas_->definirRemocaoHabilitada(!ativo);
}

void PainelVisualizacaoArvore::atualizarExecucaoAvl(
    const QString& descricao, std::size_t registroAtual,
    std::size_t totalRegistros, std::size_t numeroPasso, bool concluido)
{
    controleExecucao_->definirInformacoes(
        descricao, registroAtual, totalRegistros, numeroPasso);
    controleExecucao_->definirConcluido(concluido);
}

void PainelVisualizacaoArvore::definirReproducaoAvl(bool executando)
{
    controleExecucao_->definirExecutando(executando);
}

void PainelVisualizacaoArvore::apresentar(
    const apresentacao::arvore::ModeloVisualArvore& modelo,
    bool preservarViewport)
{
    fecharDetalhes();
    const auto& metricas = modelo.metricas();
    QString conteudo = QStringLiteral(
        "Nós: %1  •  Comparações: %2  •  Altura: %3  •  Balanceamentos (rotações): %4")
                           .arg(metricas.quantidadeNos)
                           .arg(metricas.comparacoes)
                           .arg(metricas.altura)
                           .arg(metricas.rotacoes);
    if (metricas.tempoInsercaoNanossegundos > 0) {
        conteudo.insert(
            conteudo.indexOf(QStringLiteral("  •  Comparações")),
            QStringLiteral("  •  Tempo de inserção: %1")
                .arg(formatarTempoInsercao(
                    metricas.tempoInsercaoNanossegundos)));
    }
    metricas_->definirConteudo(conteudo);
    if (!metricas.metricasPeriodicasCalculadas) {
        metricasPeriodicas_->definirMensagem(QStringLiteral(
            "Não foi possível calcular as métricas periódicas"));
    } else if (metricas.alturasPeriodicas.empty()) {
        metricasPeriodicas_->definirMensagem(QStringLiteral(
            "Alturas periódicas • Nenhuma amostra • Intervalo: 800 inserções"));
    } else {
        metricasPeriodicas_->definirResultado(
            QStringLiteral("Alturas periódicas • %1 amostras • Intervalo: 800 inserções")
                .arg(metricas.alturasPeriodicas.size()),
            formatarItensMetricasPeriodicas(metricas));
    }
    visualizacao_->apresentar(modelo, preservarViewport);
    mapa_->sincronizar();
}

void PainelVisualizacaoArvore::apresentarDetalhes(
    const apresentacao::arvore::DadosDetalhesRegistro& detalhes)
{
    if (!visualizacao_->focarRegistro(detalhes.identificador)) return;
    registroDetalhado_ = detalhes.identificador;
    popover_->apresentar(detalhes);
    reposicionarPopover();
}

void PainelVisualizacaoArvore::fecharDetalhes()
{
    const auto origem = registroDetalhado_;
    registroDetalhado_.reset();
    popover_->hide();
    if (origem) {
        const bool focado = visualizacao_->focarRegistro(*origem);
        Q_UNUSED(focado);
    }
}

void PainelVisualizacaoArvore::resizeEvent(QResizeEvent* evento)
{
    Painel::resizeEvent(evento);
    reposicionarPopover();
}

void PainelVisualizacaoArvore::reposicionarPopover()
{
    if (!registroDetalhado_ || popover_->isHidden()) return;
    const auto posicaoViewport =
        visualizacao_->posicaoRegistroNaViewport(*registroDetalhado_);
    if (!posicaoViewport) {
        fecharDetalhes();
        return;
    }

    const QPoint ancora = visualizacao_->viewport()->mapTo(
        this, *posicaoViewport);
    const QSize tamanho = popover_->sizeHint().expandedTo(popover_->minimumSize());
    const int margem = 8;
    int x = ancora.x() + static_cast<int>(visualizacao_->diametroNo() / 2.0) + 12;
    if (x + tamanho.width() > width() - margem) {
        x = ancora.x() - tamanho.width()
            - static_cast<int>(visualizacao_->diametroNo() / 2.0) - 12;
    }
    const int limiteX = std::max(margem, width() - tamanho.width() - margem);
    const int limiteY = std::max(margem, height() - tamanho.height() - margem);
    x = std::clamp(x, margem, limiteX);
    const int y = std::clamp(ancora.y() - tamanho.height() / 2,
                             margem, limiteY);
    popover_->resize(tamanho);
    popover_->move(x, y);
    popover_->raise();
}
