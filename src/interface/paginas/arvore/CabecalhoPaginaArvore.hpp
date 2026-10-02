#pragma once

#include "interface/fundamentos/BaseContainer.hpp"

class Subtitulo;
class Titulo;

/** Identificação textual da página de exploração de árvores. */
class CabecalhoPaginaArvore final : public BaseContainer
{
public:
    explicit CabecalhoPaginaArvore(QWidget* pai = nullptr);

private:
    Titulo* titulo_ = nullptr;
    Subtitulo* subtitulo_ = nullptr;
};
