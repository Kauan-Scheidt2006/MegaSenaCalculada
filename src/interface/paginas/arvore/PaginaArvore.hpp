#pragma once

#include "interface/fundamentos/BasePagina.hpp"

class ConteudoPaginaArvore;
class Painel;

/** Página inicial para exploração das estruturas de árvore. */
class PaginaArvore final : public BasePagina
{
public:
    explicit PaginaArvore(QWidget* pai = nullptr);

private:
    Painel* painelMenuEstruturas_ = nullptr;
    ConteudoPaginaArvore* conteudo_ = nullptr;
};
