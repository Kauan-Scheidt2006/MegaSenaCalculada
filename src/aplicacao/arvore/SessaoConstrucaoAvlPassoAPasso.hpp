#pragma once

#include "aplicacao/arvore/DetalhesConstrucaoModelo.hpp"
#include "estruturas/ArvoreAvlPassoAPasso.hpp"

#include <QString>

#include <cstddef>
#include <memory>
#include <vector>

namespace aplicacao::arvore {

struct ResultadoPassoAvl {
    apresentacao::arvore::ModeloVisualArvore modelo;
    QString descricao;
    std::size_t registroAtual = 0;
    std::size_t totalRegistros = 0;
    std::size_t numeroPasso = 0;
    bool concluido = false;
};

class SessaoConstrucaoAvlPassoAPasso final
{
private:
    using Arvore = estruturas::ArvoreAvlPassoAPasso<
        megasena::Registro, detalhes::ComparadorRegistroContabilizado>;

public:
    explicit SessaoConstrucaoAvlPassoAPasso(
        std::vector<megasena::Registro> registros);

    void reiniciar();
    [[nodiscard]] ResultadoPassoAvl avancar();
    [[nodiscard]] ResultadoPassoAvl estadoInicial() const;
    [[nodiscard]] bool concluida() const noexcept;

private:
    [[nodiscard]] ResultadoPassoAvl criarResultado(
        QString descricao, bool concluido = false) const;
    [[nodiscard]] QString descrever(
        const estruturas::EventoPassoAvl<megasena::Registro>& evento) const;

    std::vector<megasena::Registro> registros_;
    std::shared_ptr<std::size_t> comparacoes_;
    std::unique_ptr<Arvore> arvore_;
    std::size_t proximoRegistro_ = 0;
    std::size_t numeroPasso_ = 0;
};

} // namespace aplicacao::arvore
