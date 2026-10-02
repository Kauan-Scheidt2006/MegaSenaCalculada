#include "interface/ambiente/Ambiente.hpp"

#include "interface/ambiente/AreaPaginas.hpp"
#include "interface/ambiente/BarraEstado.hpp"
#include "interface/ambiente/EstruturaAmbiente.hpp"
#include "interface/fundamentos/HospedagemRaiz.hpp"

Ambiente::Ambiente(QWidget* pai)
    : QWidget(pai)
{
    configurarJanela();
    criarEstrutura();
}

void Ambiente::apresentar()
{
    show();
}

AreaPaginas& Ambiente::areaPaginas() const noexcept
{
    return estrutura_->areaPaginas();
}

BarraEstado& Ambiente::barraEstado() const noexcept
{
    return estrutura_->barraEstado();
}

void Ambiente::configurarJanela()
{
    setObjectName(QStringLiteral("ambiente"));
    setAttribute(Qt::WA_StyledBackground, false);
    setWindowTitle(QStringLiteral("Mega Sena Calculada"));
    setMinimumSize(960, 640);
    resize(1440, 900);
}

void Ambiente::criarEstrutura()
{
    estrutura_ = new EstruturaAmbiente(this);
    HospedagemRaiz::aplicar(*this, *estrutura_);
}
