#include "interface/ambiente/AreaPaginas.hpp"

#include "interface/fundamentos/BasePagina.hpp"

AreaPaginas::AreaPaginas(QWidget* pai)
    : BaseContainer({.componente = {.nome = QStringLiteral("areaPaginas")},
                     .disposicao = TipoDisposicao::Empilhada}, pai)
{
}

void AreaPaginas::adicionarPagina(BasePagina& pagina)
{
    adicionarComponente(pagina);
}

void AreaPaginas::exibirPagina(BasePagina& pagina)
{
    exibirComponente(pagina);
}
