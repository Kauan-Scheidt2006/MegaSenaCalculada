#pragma once

#include "interface/apresentacao/arvore/EstadoVisualizacaoArvore.hpp"
#include "interface/apresentacao/arvore/ModeloVisualArvore.hpp"

#include <QString>

#include <optional>

namespace apresentacao::arvore {

class ModeloApresentacaoArvore final {
public:
    [[nodiscard]] EstadoVisualizacaoArvore estado() const noexcept;
    [[nodiscard]] const ModeloVisualArvore* modeloVisual() const noexcept;
    [[nodiscard]] const QString& mensagemErro() const noexcept;

    void definirVazio() noexcept;
    void definirConstruindo() noexcept;
    void definirDisponivel(ModeloVisualArvore modelo);
    void definirErro(QString mensagem);
    void definirCancelado() noexcept;

private:
    void limparDados() noexcept;

    EstadoVisualizacaoArvore estado_ = EstadoVisualizacaoArvore::Vazio;
    std::optional<ModeloVisualArvore> modeloVisual_;
    QString mensagemErro_;
};

} // namespace apresentacao::arvore
