#include "aplicacao/Aplicacao.hpp"

#include "aplicacao/arvore/ConstrutorApresentacaoAvl.hpp"
#include "aplicacao/arvore/ConstrutorApresentacaoBst.hpp"
#include "aplicacao/arvore/ConstrutorApresentacaoRubroNegra.hpp"
#include "aplicacao/arvore/ConsultorDetalhesRegistro.hpp"
#include "aplicacao/arvore/ExecutorPercursosArvore.hpp"
#include "aplicacao/arvore/SessaoConstrucaoAvlPassoAPasso.hpp"
#include "infraestrutura/json/LeitorConcursosJson.hpp"
#include "interface/ambiente/Ambiente.hpp"
#include "interface/ambiente/AreaPaginas.hpp"
#include "interface/paginas/arvore/PaginaArvore.hpp"
#include "interface/paginas/arvore/TipoEstrutura.hpp"
#include "processamento/ProcessadorHistorico.hpp"

#include <QCoreApplication>
#include <QTimer>

#include <filesystem>
#include <memory>
#include <stdexcept>

namespace {

std::filesystem::path localizarConcursos()
{
    const std::filesystem::path diretorioExecutavel(
        QCoreApplication::applicationDirPath().toStdWString());
    const std::filesystem::path candidatos[]{
        std::filesystem::current_path() / "concursos.json",
        diretorioExecutavel / "concursos.json",
        diretorioExecutavel.parent_path() / "concursos.json"};
    for (const auto& candidato : candidatos) {
        if (std::filesystem::exists(candidato)) return candidato;
    }
    return candidatos[0];
}

apresentacao::arvore::ModeloVisualArvore preservarMetricasDeConstrucao(
    const apresentacao::arvore::ModeloVisualArvore& atualizado,
    const apresentacao::arvore::ModeloVisualArvore& anterior)
{
    auto metricas = anterior.metricas();
    metricas.quantidadeNos = atualizado.metricas().quantidadeNos;
    metricas.altura = atualizado.metricas().altura;
    metricas.rotacoes = atualizado.metricas().rotacoes;
    return {atualizado.raiz(), atualizado.nos(), atualizado.ligacoes(),
            std::move(metricas)};
}

} // namespace

Aplicacao::Aplicacao() = default;

Aplicacao::~Aplicacao() = default;

void Aplicacao::iniciar()
{
    if (QCoreApplication::instance() == nullptr) {
        throw std::logic_error("QApplication deve existir antes de iniciar Aplicacao.");
    }

    criarInterface();
    ambiente_->apresentar();
}

void Aplicacao::criarInterface()
{
    ambiente_ = std::make_unique<Ambiente>();
    paginaArvore_ = new PaginaArvore(&ambiente_->areaPaginas());
    ambiente_->areaPaginas().adicionarPagina(*paginaArvore_);
    ambiente_->areaPaginas().exibirPagina(*paginaArvore_);

    const megasena::infraestrutura::LeitorConcursosJson leitor;
    const megasena::processamento::ProcessadorHistorico processador;
    const auto concursos = leitor.ler(localizarConcursos());
    const auto registros = processador.processar(concursos);

    modeloBst_ = std::make_unique<apresentacao::arvore::ModeloVisualArvore>(
        aplicacao::arvore::ConstrutorApresentacaoBst{}.construir(registros));

    modeloAvl_ = std::make_unique<apresentacao::arvore::ModeloVisualArvore>(
        aplicacao::arvore::ConstrutorApresentacaoAvl{}.construir(registros));

    modeloRubroNegra_ =
        std::make_unique<apresentacao::arvore::ModeloVisualArvore>(
            aplicacao::arvore::ConstrutorApresentacaoRubroNegra{}.construir(
                registros));

    consultorDetalhes_ =
        std::make_unique<aplicacao::arvore::ConsultorDetalhesRegistro>(
            registros, concursos);
    executorPercursos_ =
        std::make_unique<aplicacao::arvore::ExecutorPercursosArvore>(
            registros);

    sessaoAvlPassoAPasso_ =
        std::make_unique<aplicacao::arvore::SessaoConstrucaoAvlPassoAPasso>(
            registros);
    temporizadorAvl_ = std::make_unique<QTimer>();
    temporizadorAvl_->setInterval(650);

    QObject::connect(
        paginaArvore_, &PaginaArvore::estruturaSelecionada, paginaArvore_,
        [this](TipoEstrutura tipo) { apresentarEstrutura(tipo); });
    QObject::connect(
        paginaArvore_, &PaginaArvore::detalhesRegistroSolicitados,
        paginaArvore_,
        [this](apresentacao::arvore::IdentificadorRegistro identificador) {
            if (const auto* detalhes = consultorDetalhes_->buscar(identificador)) {
                paginaArvore_->apresentarDetalhes(*detalhes);
            }
        });
    QObject::connect(
        paginaArvore_, &PaginaArvore::proximoPassoAvlSolicitado,
        paginaArvore_, [this] { avancarAvlPassoAPasso(); });
    QObject::connect(
        paginaArvore_, &PaginaArvore::reproducaoAvlSolicitada,
        paginaArvore_, [this] { reproduzirAvlPassoAPasso(); });
    QObject::connect(
        paginaArvore_, &PaginaArvore::pausaAvlSolicitada,
        paginaArvore_, [this] { pausarAvlPassoAPasso(); });
    QObject::connect(
        paginaArvore_, &PaginaArvore::reinicioAvlSolicitado,
        paginaArvore_, [this] { reiniciarAvlPassoAPasso(); });
    QObject::connect(
        paginaArvore_, &PaginaArvore::percursoEmOrdemSolicitado,
        paginaArvore_, [this] {
            executarPercurso(aplicacao::arvore::TipoPercursoArvore::EmOrdem);
        });
    QObject::connect(
        paginaArvore_, &PaginaArvore::percursoPreOrdemSolicitado,
        paginaArvore_, [this] {
            executarPercurso(aplicacao::arvore::TipoPercursoArvore::PreOrdem);
        });
    QObject::connect(
        paginaArvore_, &PaginaArvore::percursoPosOrdemSolicitado,
        paginaArvore_, [this] {
            executarPercurso(aplicacao::arvore::TipoPercursoArvore::PosOrdem);
        });
    QObject::connect(
        paginaArvore_, &PaginaArvore::buscaSolicitada,
        paginaArvore_, [this](auto identificador) {
            buscarRegistro(identificador);
        });
    QObject::connect(
        paginaArvore_, &PaginaArvore::remocaoSolicitada,
        paginaArvore_, [this](auto identificador) {
            removerRegistro(identificador);
        });
    QObject::connect(
        temporizadorAvl_.get(), &QTimer::timeout, paginaArvore_,
        [this] { avancarAvlPassoAPasso(); });
    apresentarEstrutura(TipoEstrutura::BuscaBinaria);
}

void Aplicacao::apresentarEstrutura(TipoEstrutura tipo)
{
    tipoEstruturaAtual_ = tipo;
    pausarAvlPassoAPasso();
    const bool passoAPasso = tipo == TipoEstrutura::AvlPassoAPasso;
    paginaArvore_->definirModoAvlPassoAPasso(passoAPasso);

    switch (tipo) {
    case TipoEstrutura::BuscaBinaria:
        paginaArvore_->apresentar(*modeloBst_);
        break;
    case TipoEstrutura::Avl:
        paginaArvore_->apresentar(*modeloAvl_);
        break;
    case TipoEstrutura::RubroNegra:
        paginaArvore_->apresentar(*modeloRubroNegra_);
        break;
    case TipoEstrutura::AvlPassoAPasso:
        reiniciarAvlPassoAPasso();
        break;
    default:
        throw std::invalid_argument("Tipo de estrutura desconhecido.");
    }
}

void Aplicacao::executarPercurso(
    aplicacao::arvore::TipoPercursoArvore percurso)
{
    const auto resultado = executorPercursos_->executar(
        tipoEstruturaAtual_, percurso);
    std::vector<apresentacao::arvore::IdentificadorRegistro> ordem;
    ordem.reserve(resultado.registros.size());
    for (const auto& registro : resultado.registros) {
        ordem.push_back({registro.chave.frequenciaAcumulada,
                         registro.chave.numero});
    }

    QString nomePercurso;
    switch (percurso) {
    case aplicacao::arvore::TipoPercursoArvore::EmOrdem:
        nomePercurso = QStringLiteral("Percurso em ordem");
        break;
    case aplicacao::arvore::TipoPercursoArvore::PreOrdem:
        nomePercurso = QStringLiteral("Percurso em pré-ordem");
        break;
    case aplicacao::arvore::TipoPercursoArvore::PosOrdem:
        nomePercurso = QStringLiteral("Percurso em pós-ordem");
        break;
    }
    paginaArvore_->apresentarResultadoPercurso(
        nomePercurso, ordem, resultado.quantidadeVisitada,
        resultado.tempoExecucaoNanossegundos);
}

void Aplicacao::buscarRegistro(
    apresentacao::arvore::IdentificadorRegistro identificador)
{
    const auto resultado = executorPercursos_->buscar(
        tipoEstruturaAtual_,
        {identificador.frequenciaAcumulada, identificador.numero});
    paginaArvore_->apresentarResultadoBusca(
        identificador, resultado.registro.has_value(),
        resultado.nosVisitados, resultado.comparacoes,
        resultado.tempoExecucaoNanossegundos);
}

void Aplicacao::removerRegistro(
    apresentacao::arvore::IdentificadorRegistro identificador)
{
    if (tipoEstruturaAtual_ == TipoEstrutura::AvlPassoAPasso) {
        paginaArvore_->apresentarResultadoRemocao(
            identificador, false, 0, 0, 0);
        return;
    }

    const auto resultado = executorPercursos_->remover(
        tipoEstruturaAtual_,
        {identificador.frequenciaAcumulada, identificador.numero});

    if (resultado.removido) {
        auto modelo = executorPercursos_->criarModeloVisual(tipoEstruturaAtual_);
        switch (tipoEstruturaAtual_) {
        case TipoEstrutura::BuscaBinaria:
            modelo = preservarMetricasDeConstrucao(modelo, *modeloBst_);
            modeloBst_ = std::make_unique<apresentacao::arvore::ModeloVisualArvore>(
                std::move(modelo));
            paginaArvore_->apresentar(*modeloBst_, true);
            break;
        case TipoEstrutura::Avl:
            modelo = preservarMetricasDeConstrucao(modelo, *modeloAvl_);
            modeloAvl_ = std::make_unique<apresentacao::arvore::ModeloVisualArvore>(
                std::move(modelo));
            paginaArvore_->apresentar(*modeloAvl_, true);
            break;
        case TipoEstrutura::RubroNegra:
            modelo = preservarMetricasDeConstrucao(modelo, *modeloRubroNegra_);
            modeloRubroNegra_ =
                std::make_unique<apresentacao::arvore::ModeloVisualArvore>(
                    std::move(modelo));
            paginaArvore_->apresentar(*modeloRubroNegra_, true);
            break;
        case TipoEstrutura::AvlPassoAPasso:
            break;
        }
    }

    paginaArvore_->apresentarResultadoRemocao(
        identificador, resultado.removido, resultado.nosVisitados,
        resultado.comparacoes,
        resultado.tempoExecucaoNanossegundos);
}

void Aplicacao::avancarAvlPassoAPasso()
{
    const auto resultado = sessaoAvlPassoAPasso_->avancar();
    paginaArvore_->apresentar(resultado.modelo, true);
    paginaArvore_->atualizarExecucaoAvl(
        resultado.descricao, resultado.registroAtual,
        resultado.totalRegistros, resultado.numeroPasso, resultado.concluido);
    if (resultado.concluido) pausarAvlPassoAPasso();
}

void Aplicacao::reproduzirAvlPassoAPasso()
{
    if (sessaoAvlPassoAPasso_->concluida()) return;
    paginaArvore_->definirReproducaoAvl(true);
    temporizadorAvl_->start();
}

void Aplicacao::pausarAvlPassoAPasso()
{
    if (temporizadorAvl_) temporizadorAvl_->stop();
    if (paginaArvore_) paginaArvore_->definirReproducaoAvl(false);
}

void Aplicacao::reiniciarAvlPassoAPasso()
{
    pausarAvlPassoAPasso();
    sessaoAvlPassoAPasso_->reiniciar();
    const auto resultado = sessaoAvlPassoAPasso_->estadoInicial();
    paginaArvore_->apresentar(resultado.modelo);
    paginaArvore_->atualizarExecucaoAvl(
        resultado.descricao, resultado.registroAtual,
        resultado.totalRegistros, resultado.numeroPasso, resultado.concluido);
}
