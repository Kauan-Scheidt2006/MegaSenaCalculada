#pragma once

#include "interface/fundamentos/BaseContainer.hpp"
#include "interface/paginas/arvore/TipoEstrutura.hpp"

class Subtitulo;
class Titulo;

/** Identificação textual da página de exploração de árvores. */
class CabecalhoPaginaArvore final : public BaseContainer
{
public:
    explicit CabecalhoPaginaArvore(QWidget* pai = nullptr);

    void definirTipoEstrutura(TipoEstrutura tipo);

private:
    Titulo* titulo_ = nullptr;
    Subtitulo* subtitulo_ = nullptr;
};
