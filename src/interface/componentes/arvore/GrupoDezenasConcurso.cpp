#include "interface/componentes/arvore/GrupoDezenasConcurso.hpp"

#include "interface/componentes/arvore/MarcadorDezena.hpp"

GrupoDezenasConcurso::GrupoDezenasConcurso(QWidget* pai)
    : BaseContainer({.componente = {.nomeObjeto = QStringLiteral("grupoDezenasConcurso"),
                                    .nomeAcessivel = QStringLiteral("Dezenas do concurso"),
                                    .fundoEstilizado = false},
                     .disposicao = TipoDisposicao::Horizontal,
                     .espacamento = 6}, pai)
{
    for (std::size_t indice = 0; indice < marcadores_.size(); ++indice) {
        marcadores_[indice] = new MarcadorDezena(static_cast<int>(indice + 1), this);
        adicionarComponente(*marcadores_[indice]);
    }
}

void GrupoDezenasConcurso::apresentar(const std::array<int, 6>& dezenas,
                                      int dezenaCorrespondente)
{
    for (std::size_t indice = 0; indice < dezenas.size(); ++indice) {
        marcadores_[indice]->apresentar(
            dezenas[indice], dezenas[indice] == dezenaCorrespondente);
    }
}
