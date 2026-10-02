#include "interface/ambiente/EstruturaAmbiente.hpp"

#include "interface/ambiente/AreaPaginas.hpp"
#include "interface/ambiente/BarraEstado.hpp"

EstruturaAmbiente::EstruturaAmbiente(QWidget* pai)
    : BaseContainer({.componente = {.nomeObjeto = QStringLiteral("estruturaAmbiente"),
                                    .fundoEstilizado = false},
                     .disposicao = TipoDisposicao::Vertical}, pai),
      areaPaginas_(new AreaPaginas(this)),
      barraEstado_(new BarraEstado(this))
{
    adicionarComponente(*areaPaginas_, 1);
    adicionarComponente(*barraEstado_);
}

AreaPaginas& EstruturaAmbiente::areaPaginas() const noexcept
{
    return *areaPaginas_;
}

BarraEstado& EstruturaAmbiente::barraEstado() const noexcept
{
    return *barraEstado_;
}
