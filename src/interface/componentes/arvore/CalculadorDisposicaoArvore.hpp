#pragma once

#include "interface/apresentacao/arvore/ModeloVisualArvore.hpp"

#include <QPointF>
#include <QRectF>

#include <map>

namespace interface_grafica::arvore {

struct ParametrosDisposicaoArvore {
    qreal diametroNo;
    qreal espacamentoHorizontal;
    qreal espacamentoVertical;
};

class DisposicaoArvore final {
public:
    DisposicaoArvore(
        std::map<apresentacao::arvore::IdentificadorRegistro, QPointF> posicoes,
        QRectF limites);

    [[nodiscard]] const QPointF& posicao(
        const apresentacao::arvore::IdentificadorRegistro& identificador) const;
    [[nodiscard]] const std::map<apresentacao::arvore::IdentificadorRegistro,
                                 QPointF>& posicoes() const noexcept;
    [[nodiscard]] const QRectF& limites() const noexcept;

private:
    std::map<apresentacao::arvore::IdentificadorRegistro, QPointF> posicoes_;
    QRectF limites_;
};

class CalculadorDisposicaoArvore final {
public:
    [[nodiscard]] DisposicaoArvore calcular(
        const apresentacao::arvore::ModeloVisualArvore& modelo,
        ParametrosDisposicaoArvore parametros) const;
};

} // namespace interface_grafica::arvore
