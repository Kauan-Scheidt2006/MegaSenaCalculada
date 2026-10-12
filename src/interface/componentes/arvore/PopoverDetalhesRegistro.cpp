#include "interface/componentes/arvore/PopoverDetalhesRegistro.hpp"

#include "interface/componentes/arvore/CampoDetalhe.hpp"
#include "interface/componentes/arvore/GrupoDezenasConcurso.hpp"
#include "interface/componentes/basicos/Botao.hpp"
#include "interface/componentes/basicos/Texto.hpp"
#include "interface/fundamentos/BaseContainer.hpp"

#include <QKeyEvent>
#include <QSizePolicy>

namespace {

class CabecalhoPopover final : public BaseContainer
{
public:
    CabecalhoPopover(QWidget& titulo, QWidget& fechar, QWidget* pai)
        : BaseContainer({.componente = {.nomeObjeto = QStringLiteral("cabecalhoPopoverDetalhes"),
                                        .fundoEstilizado = false},
                         .disposicao = TipoDisposicao::Horizontal,
                         .espacamento = 8}, pai)
    {
        adicionarComponente(titulo, 1);
        adicionarComponente(fechar);
    }
};

} // namespace

PopoverDetalhesRegistro::PopoverDetalhesRegistro(QWidget* pai)
    : Painel({.componente = {.nomeObjeto = QStringLiteral("popoverDetalhesRegistro"),
                             .nomeAcessivel = QStringLiteral("Detalhes do registro"),
                             .tamanhoMinimo = QSize(330, 0),
                             .tamanhoMaximo = QSize(420, 360),
                             .politicaTamanho = QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum),
                             .fundoEstilizado = true,
                             .visivel = false},
              .disposicao = TipoDisposicao::Vertical,
              .margens = {16, 14, 16, 16},
              .espacamento = 10}, pai),
      titulo_(new Texto(QStringLiteral("Detalhes do registro"),
                        {.nomeObjeto = QStringLiteral("tituloPopoverDetalhes"),
                         .fundoEstilizado = false}, this)),
      fechar_(new Botao(QStringLiteral("Fechar"),
                        {.nomeObjeto = QStringLiteral("fecharPopoverDetalhes"),
                         .nomeAcessivel = QStringLiteral("Fechar detalhes"),
                         .fundoEstilizado = true}, this)),
      dezena_(new CampoDetalhe(QStringLiteral("campoDezenaDetalhes"),
                               QStringLiteral("Dezena"), this)),
      frequencia_(new CampoDetalhe(QStringLiteral("campoFrequenciaDetalhes"),
                                   QStringLiteral("Frequência acumulada"), this)),
      concurso_(new CampoDetalhe(QStringLiteral("campoConcursoDetalhes"),
                                 QStringLiteral("Concurso"), this)),
      data_(new CampoDetalhe(QStringLiteral("campoDataDetalhes"),
                             QStringLiteral("Data"), this)),
      dezenas_(new GrupoDezenasConcurso(this))
{
    auto* cabecalho = new CabecalhoPopover(*titulo_, *fechar_, this);

    adicionarComponente(*cabecalho);
    adicionarComponente(*dezena_);
    adicionarComponente(*frequencia_);
    adicionarComponente(*concurso_);
    adicionarComponente(*data_);
    adicionarComponente(*dezenas_);
    setFocusPolicy(Qt::StrongFocus);
    fechar_->installEventFilter(this);
    connect(fechar_, &Botao::clicked,
            this, &PopoverDetalhesRegistro::fechamentoSolicitado);
}

bool PopoverDetalhesRegistro::eventFilter(QObject* observado, QEvent* evento)
{
    if (observado == fechar_ && evento->type() == QEvent::KeyPress
        && static_cast<QKeyEvent*>(evento)->key() == Qt::Key_Escape) {
        emit fechamentoSolicitado();
        return true;
    }
    return Painel::eventFilter(observado, evento);
}

void PopoverDetalhesRegistro::apresentar(
    const apresentacao::arvore::DadosDetalhesRegistro& detalhes)
{
    dezena_->definirValor(QStringLiteral("%1").arg(
        detalhes.identificador.numero, 2, 10, QLatin1Char('0')));
    frequencia_->definirValor(
        QStringLiteral("%1 vezes").arg(detalhes.identificador.frequenciaAcumulada));
    concurso_->definirValor(QString::number(detalhes.numeroConcurso));
    data_->definirValor(detalhes.dataConcurso);
    dezenas_->apresentar(detalhes.dezenas, detalhes.identificador.numero);
    setAccessibleDescription(
        QStringLiteral("Dezena %1, sorteada %2 vezes ate o concurso %3 de %4.")
            .arg(detalhes.identificador.numero)
            .arg(detalhes.identificador.frequenciaAcumulada)
            .arg(detalhes.numeroConcurso)
            .arg(detalhes.dataConcurso));
    adjustSize();
    show();
    raise();
    fechar_->setFocus(Qt::OtherFocusReason);
}

void PopoverDetalhesRegistro::keyPressEvent(QKeyEvent* evento)
{
    if (evento->key() == Qt::Key_Escape) {
        emit fechamentoSolicitado();
        evento->accept();
        return;
    }
    Painel::keyPressEvent(evento);
}
