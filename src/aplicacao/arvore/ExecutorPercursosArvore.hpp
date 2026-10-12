#pragma once

#include "dominio/Registro.hpp"
#include "estruturas/ArvoreAvl.hpp"
#include "estruturas/ArvoreBinariaBusca.hpp"
#include "estruturas/ArvoreRubroNegra.hpp"
#include "interface/paginas/arvore/TipoEstrutura.hpp"
#include "interface/apresentacao/arvore/ModeloVisualArvore.hpp"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>

namespace aplicacao::arvore {

enum class TipoPercursoArvore {
    EmOrdem,
    PreOrdem,
    PosOrdem,
};

struct ResultadoPercursoArvore {
    std::vector<megasena::Registro> registros;
    std::size_t quantidadeVisitada = 0;
    std::uint64_t tempoExecucaoNanossegundos = 0;
};

struct ResultadoBuscaArvore {
    std::optional<megasena::Registro> registro;
    std::size_t nosVisitados = 0;
    std::size_t comparacoes = 0;
    std::uint64_t tempoExecucaoNanossegundos = 0;
};

struct ResultadoRemocaoArvore {
    bool removido = false;
    std::size_t nosVisitados = 0;
    std::size_t comparacoes = 0;
    std::uint64_t tempoExecucaoNanossegundos = 0;
};

class ExecutorPercursosArvore final {
public:
    explicit ExecutorPercursosArvore(
        const std::vector<megasena::Registro>& registros);

    [[nodiscard]] ResultadoPercursoArvore executar(
        TipoEstrutura estrutura, TipoPercursoArvore percurso) const;
    [[nodiscard]] ResultadoBuscaArvore buscar(
        TipoEstrutura estrutura, const megasena::Chave& chave) const;
    [[nodiscard]] ResultadoRemocaoArvore remover(
        TipoEstrutura estrutura, const megasena::Chave& chave);
    [[nodiscard]] apresentacao::arvore::ModeloVisualArvore criarModeloVisual(
        TipoEstrutura estrutura) const;

private:
    using Bst = estruturas::ArvoreBinariaBusca<
        megasena::Registro, megasena::ComparadorRegistro>;
    using Avl = estruturas::ArvoreAvl<
        megasena::Registro, megasena::ComparadorRegistro>;
    using RubroNegra = estruturas::ArvoreRubroNegra<
        megasena::Registro, megasena::ComparadorRegistro>;

    Bst bst_;
    Avl avl_;
    RubroNegra rubroNegra_;
};

} // namespace aplicacao::arvore
