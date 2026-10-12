#pragma once

#include "interface/fundamentos/BaseContainer.hpp"

class Texto;

class CampoDetalhe final : public BaseContainer
{
    Q_OBJECT

public:
    CampoDetalhe(const QString& nomeObjeto, const QString& rotulo,
                 QWidget* pai = nullptr);

    void definirValor(const QString& valor);
    [[nodiscard]] QString valor() const;

private:
    Texto* rotulo_ = nullptr;
    Texto* valor_ = nullptr;
};
