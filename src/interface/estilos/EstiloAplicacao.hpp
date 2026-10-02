#pragma once

class QApplication;

/** Carrega e instala a folha QSS única antes da criação dos componentes visuais. */
class EstiloAplicacao final
{
public:
    static void aplicar(QApplication& aplicacao);

    EstiloAplicacao() = delete;
};
