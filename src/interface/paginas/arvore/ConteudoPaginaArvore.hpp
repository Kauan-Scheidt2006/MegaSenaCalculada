#pragma once

#include "interface/fundamentos/BaseContainer.hpp"

class CabecalhoPaginaArvore;
class PainelVisualizacaoArvore;

/** Organiza o cabeçalho e a região principal da página da árvore. */
class ConteudoPaginaArvore final : public BaseContainer
{
public:
    explicit ConteudoPaginaArvore(QWidget* pai = nullptr);

private:
    CabecalhoPaginaArvore* cabecalho_ = nullptr;
    PainelVisualizacaoArvore* painelVisualizacao_ = nullptr;
};
