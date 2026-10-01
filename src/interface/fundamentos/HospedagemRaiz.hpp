#pragma once

class QWidget;

/** Instala um único componente especializado para ocupar integralmente um widget hospedeiro. */
class HospedagemRaiz final
{
public:
    static void aplicar(QWidget& hospedeiro, QWidget& conteudo);

    HospedagemRaiz() = delete;
};
