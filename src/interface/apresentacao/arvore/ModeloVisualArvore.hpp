#pragma once

#include "interface/apresentacao/arvore/ModeloLigacaoArvore.hpp"
#include "interface/apresentacao/arvore/ModeloNoArvore.hpp"
#include "dominio/AmostraAlturaArvore.hpp"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>

namespace apresentacao::arvore {

struct MetricasArvore {
    std::size_t quantidadeNos = 0;
    std::size_t altura = 0;
    std::size_t comparacoes = 0;
    std::size_t rotacoes = 0;
    std::uint64_t tempoInsercaoNanossegundos = 0;
    bool metricasPeriodicasCalculadas = false;
    std::vector<megasena::AmostraAlturaArvore> alturasPeriodicas;
};

class ModeloVisualArvore final {
public:
    ModeloVisualArvore() = default;
    ModeloVisualArvore(std::optional<IdentificadorRegistro> raiz,
                       std::vector<ModeloNoArvore> nos,
                       std::vector<ModeloLigacaoArvore> ligacoes,
                       MetricasArvore metricas = {});

    [[nodiscard]] bool vazia() const noexcept;
    [[nodiscard]] const std::optional<IdentificadorRegistro>& raiz() const noexcept;
    [[nodiscard]] const std::vector<ModeloNoArvore>& nos() const noexcept;
    [[nodiscard]] const std::vector<ModeloLigacaoArvore>& ligacoes() const noexcept;
    [[nodiscard]] const MetricasArvore& metricas() const noexcept;
    [[nodiscard]] const ModeloNoArvore* buscarNo(
        const IdentificadorRegistro& identificador) const noexcept;

private:
    std::optional<IdentificadorRegistro> raiz_;
    std::vector<ModeloNoArvore> nos_;
    std::vector<ModeloLigacaoArvore> ligacoes_;
    MetricasArvore metricas_;
};

} // namespace apresentacao::arvore
