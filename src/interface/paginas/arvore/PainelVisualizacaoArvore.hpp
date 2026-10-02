#pragma once

#include "interface/fundamentos/Painel.hpp"

/** Região reservada à apresentação da árvore, inicialmente sem conteúdo. */
class PainelVisualizacaoArvore final : public Painel
{
public:
    explicit PainelVisualizacaoArvore(QWidget* pai = nullptr);
};
