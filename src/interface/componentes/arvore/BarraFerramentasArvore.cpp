#include "interface/componentes/arvore/BarraFerramentasArvore.hpp"

#include "interface/componentes/arvore/ControleZoom.hpp"
#include "interface/componentes/basicos/Botao.hpp"

#include <QSizePolicy>
#include <QLineEdit>
#include <QRegularExpression>

BarraFerramentasArvore::BarraFerramentasArvore(QWidget* pai)
    : Barra({.componente = {.nomeObjeto = QStringLiteral("barraFerramentasArvore"),
                            .nomeAcessivel = QStringLiteral("Ferramentas da arvore"),
                            .politicaTamanho = QSizePolicy(QSizePolicy::Expanding,
                                                          QSizePolicy::Maximum),
                            .fundoEstilizado = false},
             .disposicao = TipoDisposicao::Horizontal,
             .espacamento = 8}, pai),
      controleZoom_(new ControleZoom(this)),
      enquadrar_(new Botao(
          QStringLiteral("Enquadrar"),
          {.nomeObjeto = QStringLiteral("enquadrarArvore"),
           .nomeAcessivel = QStringLiteral("Enquadrar arvore inteira"),
           .fundoEstilizado = true}, this)),
      arrastar_(new Botao(
          QStringLiteral("Arrastar"),
          {.nomeObjeto = QStringLiteral("modoArrastoArvore"),
           .nomeAcessivel = QStringLiteral("Ativar ou desativar modo de arrasto"),
           .fundoEstilizado = true}, this)),
      foco_(new Botao(
          QStringLiteral("Foco"),
          {.nomeObjeto = QStringLiteral("modoFocoArvore"),
           .nomeAcessivel = QStringLiteral("Mostrar caminho e contexto do no selecionado"),
           .fundoEstilizado = true}, this)),
      expandirTudo_(new Botao(
          QStringLiteral("Expandir tudo"),
          {.nomeObjeto = QStringLiteral("expandirTodaArvore"),
           .nomeAcessivel = QStringLiteral("Expandir todas as subarvores recolhidas"),
           .fundoEstilizado = true}, this)),
      recolherSelecao_(new Botao(
          QStringLiteral("Recolher/expandir"),
          {.nomeObjeto = QStringLiteral("recolherSelecaoArvore"),
           .nomeAcessivel = QStringLiteral("Recolher ou expandir os descendentes do no selecionado"),
           .fundoEstilizado = true}, this)),
      percorrerEmOrdem_(new Botao(
          QStringLiteral("Em ordem"),
          {.nomeObjeto = QStringLiteral("percorrerEmOrdemArvore"),
           .nomeAcessivel = QStringLiteral("Percorrer arvore em ordem"),
           .fundoEstilizado = true}, this)),
      percorrerPreOrdem_(new Botao(
          QStringLiteral("Pré-ordem"),
          {.nomeObjeto = QStringLiteral("percorrerPreOrdemArvore"),
           .nomeAcessivel = QStringLiteral("Percorrer arvore em pre-ordem"),
           .fundoEstilizado = true}, this)),
      percorrerPosOrdem_(new Botao(
          QStringLiteral("Pós-ordem"),
          {.nomeObjeto = QStringLiteral("percorrerPosOrdemArvore"),
           .nomeAcessivel = QStringLiteral("Percorrer arvore em pos-ordem"),
           .fundoEstilizado = true}, this)),
      chaveBusca_(new QLineEdit(this)),
      buscar_(new Botao(
          QStringLiteral("Buscar"),
          {.nomeObjeto = QStringLiteral("buscarNaArvore"),
           .nomeAcessivel = QStringLiteral("Buscar chave na arvore"),
           .fundoEstilizado = true}, this)),
      remover_(new Botao(
          QStringLiteral("Remover"),
          {.nomeObjeto = QStringLiteral("removerDaArvore"),
           .nomeAcessivel = QStringLiteral("Remover chave da arvore"),
           .fundoEstilizado = true}, this))
{
    chaveBusca_->setObjectName(QStringLiteral("chaveBuscaArvore"));
    chaveBusca_->setAccessibleName(
        QStringLiteral("Chave completa para busca: frequência e número"));
    chaveBusca_->setAccessibleDescription(QStringLiteral(
        "Informe os dois componentes da chave separados por vírgula."));
    chaveBusca_->setPlaceholderText(QStringLiteral("frequência,número"));
    chaveBusca_->setToolTip(
        QStringLiteral("Informe a chave como frequência,número. Exemplo: 3,27"));
    chaveBusca_->setMaximumWidth(160);
    arrastar_->setCheckable(true);
    arrastar_->setChecked(true);
    arrastar_->setAccessibleDescription(QStringLiteral("Ativado"));
    foco_->setCheckable(true);
    adicionarComponente(*controleZoom_);
    adicionarComponente(*enquadrar_);
    adicionarComponente(*arrastar_);
    adicionarComponente(*foco_);
    adicionarComponente(*recolherSelecao_);
    adicionarComponente(*expandirTudo_);
    adicionarComponente(*percorrerEmOrdem_);
    adicionarComponente(*percorrerPreOrdem_);
    adicionarComponente(*percorrerPosOrdem_);
    adicionarComponente(*chaveBusca_);
    adicionarComponente(*buscar_);
    adicionarComponente(*remover_);
    adicionarEspacamentoExpansivel(1);

    connect(controleZoom_, &ControleZoom::aumentoSolicitado,
            this, &BarraFerramentasArvore::aumentoZoomSolicitado);
    connect(controleZoom_, &ControleZoom::reducaoSolicitada,
            this, &BarraFerramentasArvore::reducaoZoomSolicitada);
    connect(enquadrar_, &Botao::clicked,
            this, &BarraFerramentasArvore::enquadramentoSolicitado);
    connect(arrastar_, &Botao::toggled, this,
            [this](bool ativo) {
                arrastar_->setAccessibleDescription(
                    ativo ? QStringLiteral("Ativado")
                          : QStringLiteral("Desativado"));
                emit modoArrastoAlterado(ativo);
            });
    connect(foco_, &Botao::toggled,
            this, &BarraFerramentasArvore::modoFocoAlterado);
    connect(expandirTudo_, &Botao::clicked,
            this, &BarraFerramentasArvore::expansaoTotalSolicitada);
    connect(recolherSelecao_, &Botao::clicked,
            this, &BarraFerramentasArvore::recolhimentoSelecaoSolicitado);
    connect(percorrerEmOrdem_, &Botao::clicked,
            this, &BarraFerramentasArvore::percursoEmOrdemSolicitado);
    connect(percorrerPreOrdem_, &Botao::clicked,
            this, &BarraFerramentasArvore::percursoPreOrdemSolicitado);
    connect(percorrerPosOrdem_, &Botao::clicked,
            this, &BarraFerramentasArvore::percursoPosOrdemSolicitado);
    connect(buscar_, &Botao::clicked,
            this, &BarraFerramentasArvore::solicitarBusca);
    connect(chaveBusca_, &QLineEdit::returnPressed,
            this, &BarraFerramentasArvore::solicitarBusca);
    connect(remover_, &Botao::clicked, this, [this] {
        static const QRegularExpression formato(
            QStringLiteral(R"(^\s*(\d+)\s*,\s*(\d+)\s*$)"));
        const auto correspondencia = formato.match(chaveBusca_->text());
        bool frequenciaValida = false;
        bool numeroValido = false;
        const int frequencia = correspondencia.hasMatch()
            ? correspondencia.captured(1).toInt(&frequenciaValida) : 0;
        const int numero = correspondencia.hasMatch()
            ? correspondencia.captured(2).toInt(&numeroValido) : 0;
        if (!correspondencia.hasMatch() || !frequenciaValida || !numeroValido
            || frequencia <= 0 || numero <= 0 || numero > 60) {
            emit buscaInvalida(QStringLiteral(
                "Chave inválida. Use frequência,número; por exemplo, 3,27."));
            chaveBusca_->setFocus();
            chaveBusca_->selectAll();
            return;
        }
        emit remocaoSolicitada({frequencia, numero});
    });
}

void BarraFerramentasArvore::solicitarBusca()
{
    static const QRegularExpression formato(
        QStringLiteral(R"(^\s*(\d+)\s*,\s*(\d+)\s*$)"));
    const auto correspondencia = formato.match(chaveBusca_->text());
    if (!correspondencia.hasMatch()) {
        emit buscaInvalida(QStringLiteral(
            "Chave inválida. Use o formato frequência,número; por exemplo, 3,27."));
        chaveBusca_->setFocus();
        chaveBusca_->selectAll();
        return;
    }

    bool frequenciaValida = false;
    bool numeroValido = false;
    const int frequencia = correspondencia.captured(1).toInt(&frequenciaValida);
    const int numero = correspondencia.captured(2).toInt(&numeroValido);
    if (!frequenciaValida || !numeroValido || frequencia <= 0
        || numero <= 0 || numero > 60) {
        emit buscaInvalida(QStringLiteral(
            "Chave inválida. Frequência e número devem ser positivos, e o número deve estar entre 1 e 60."));
        chaveBusca_->setFocus();
        chaveBusca_->selectAll();
        return;
    }

    emit buscaSolicitada({frequencia, numero});
}

void BarraFerramentasArvore::definirPercentualZoom(int percentual)
{
    controleZoom_->definirPercentual(percentual);
}

void BarraFerramentasArvore::definirRemocaoHabilitada(bool habilitada)
{
    remover_->setEnabled(habilitada);
    remover_->setToolTip(
        habilitada
            ? QStringLiteral("Remove a chave da estrutura selecionada.")
            : QStringLiteral("A remoção não está disponível na AVL passo a passo."));
    remover_->setAccessibleDescription(remover_->toolTip());
}

bool BarraFerramentasArvore::modoArrastoAtivo() const noexcept
{
    return arrastar_->isChecked();
}
