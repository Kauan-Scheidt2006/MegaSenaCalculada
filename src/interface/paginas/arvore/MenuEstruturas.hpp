#pragma once

#include "interface/fundamentos/BaseContainer.hpp"
#include "interface/paginas/arvore/TipoEstrutura.hpp"

class ItemNavegacaoEstrutura;
class Texto;

/** Menu contextual que comunica a estrutura solicitada sem construir arvores. */
class MenuEstruturas final : public BaseContainer
{
    Q_OBJECT

public:
    explicit MenuEstruturas(QWidget* pai = nullptr);

    [[nodiscard]] TipoEstrutura tipoSelecionado() const noexcept;
    void selecionar(TipoEstrutura tipo);

signals:
    void estruturaSelecionada(TipoEstrutura tipo);

private:
    void tratarAcionamento(ItemNavegacaoEstrutura& item);

    Texto* titulo_ = nullptr;
    ItemNavegacaoEstrutura* itemBuscaBinaria_ = nullptr;
    ItemNavegacaoEstrutura* itemAvl_ = nullptr;
    ItemNavegacaoEstrutura* itemRubroNegra_ = nullptr;
    ItemNavegacaoEstrutura* itemAvlPassoAPasso_ = nullptr;
    TipoEstrutura tipoSelecionado_ = TipoEstrutura::BuscaBinaria;
};
