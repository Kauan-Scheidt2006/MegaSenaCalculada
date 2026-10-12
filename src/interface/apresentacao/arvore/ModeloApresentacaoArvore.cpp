#include "interface/apresentacao/arvore/ModeloApresentacaoArvore.hpp"

#include <stdexcept>
#include <utility>

namespace apresentacao::arvore {

EstadoVisualizacaoArvore ModeloApresentacaoArvore::estado() const noexcept
{
    return estado_;
}

const ModeloVisualArvore* ModeloApresentacaoArvore::modeloVisual() const noexcept
{
    return modeloVisual_ ? &*modeloVisual_ : nullptr;
}

const QString& ModeloApresentacaoArvore::mensagemErro() const noexcept
{
    return mensagemErro_;
}

void ModeloApresentacaoArvore::definirVazio() noexcept
{
    limparDados();
    estado_ = EstadoVisualizacaoArvore::Vazio;
}

void ModeloApresentacaoArvore::definirConstruindo() noexcept
{
    limparDados();
    estado_ = EstadoVisualizacaoArvore::Construindo;
}

void ModeloApresentacaoArvore::definirDisponivel(ModeloVisualArvore modelo)
{
    if (modelo.vazia()) {
        throw std::invalid_argument(
            "O estado disponivel exige uma arvore visual nao vazia.");
    }
    limparDados();
    modeloVisual_.emplace(std::move(modelo));
    estado_ = EstadoVisualizacaoArvore::Disponivel;
}

void ModeloApresentacaoArvore::definirErro(QString mensagem)
{
    if (mensagem.trimmed().isEmpty()) {
        throw std::invalid_argument("O estado de erro exige uma mensagem.");
    }
    limparDados();
    mensagemErro_ = std::move(mensagem);
    estado_ = EstadoVisualizacaoArvore::Erro;
}

void ModeloApresentacaoArvore::definirCancelado() noexcept
{
    limparDados();
    estado_ = EstadoVisualizacaoArvore::Cancelado;
}

void ModeloApresentacaoArvore::limparDados() noexcept
{
    modeloVisual_.reset();
    mensagemErro_.clear();
}

} // namespace apresentacao::arvore
