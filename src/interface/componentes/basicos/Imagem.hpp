#pragma once

#include "interface/parametros/ParametrosComponente.hpp"

#include <QLabel>

class Imagem final : public QLabel
{
public:
    explicit Imagem(const QString& recurso, const QString& textoAlternativo,
                    const ParametrosComponente& parametros, QWidget* pai = nullptr);

    void definirRecurso(const QString& recurso);
    void definirTextoAlternativo(const QString& textoAlternativo);
};
