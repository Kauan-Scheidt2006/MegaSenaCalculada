#include "interface/componentes/arvore/MarcadorDezena.hpp"

#include <QStyle>

MarcadorDezena::MarcadorDezena(int indice, QWidget* pai)
    : Texto(QStringLiteral("00"),
            {.nomeObjeto = QStringLiteral("marcadorDezena%1").arg(indice),
             .nomeAcessivel = QStringLiteral("Dezena do concurso"),
             .fundoEstilizado = true}, pai)
{
    setAlignment(Qt::AlignCenter);
    setProperty("correspondente", false);
}

void MarcadorDezena::apresentar(int dezena, bool correspondente)
{
    dezena_ = dezena;
    correspondente_ = correspondente;
    definirConteudo(QStringLiteral("%1").arg(dezena, 2, 10, QLatin1Char('0')));
    setAccessibleDescription(correspondente
        ? QStringLiteral("Dezena correspondente ao no selecionado")
        : QStringLiteral("Dezena do concurso"));
    setProperty("correspondente", correspondente);
    style()->unpolish(this);
    style()->polish(this);
}

int MarcadorDezena::dezena() const noexcept { return dezena_; }
bool MarcadorDezena::correspondente() const noexcept { return correspondente_; }
