#pragma once

#include <memory>

class Ambiente;
class PaginaArvore;
class QTimer;
enum class TipoEstrutura;

namespace apresentacao::arvore {
struct IdentificadorRegistro;
class ModeloVisualArvore;
}

namespace aplicacao::arvore {
class ConsultorDetalhesRegistro;
class ExecutorPercursosArvore;
class SessaoConstrucaoAvlPassoAPasso;
enum class TipoPercursoArvore;
}

/**
 * Raiz de composição e proprietária dos componentes de alto nível do programa.
 * Novos serviços e a interface serão mantidos vivos por esta classe conforme
 * as etapas seguintes da arquitetura forem implementadas.
 */
class Aplicacao final
{
public:
    Aplicacao();
    ~Aplicacao();

    Aplicacao(const Aplicacao&) = delete;
    Aplicacao& operator=(const Aplicacao&) = delete;
    Aplicacao(Aplicacao&&) = delete;
    Aplicacao& operator=(Aplicacao&&) = delete;

    void iniciar();

private:
    void criarInterface();
    void apresentarEstrutura(TipoEstrutura tipo);
    void avancarAvlPassoAPasso();
    void reproduzirAvlPassoAPasso();
    void pausarAvlPassoAPasso();
    void reiniciarAvlPassoAPasso();
    void executarPercurso(aplicacao::arvore::TipoPercursoArvore percurso);
    void buscarRegistro(
        apresentacao::arvore::IdentificadorRegistro identificador);
    void removerRegistro(
        apresentacao::arvore::IdentificadorRegistro identificador);

    std::unique_ptr<Ambiente> ambiente_;
    PaginaArvore* paginaArvore_ = nullptr;
    std::unique_ptr<apresentacao::arvore::ModeloVisualArvore> modeloBst_;
    std::unique_ptr<apresentacao::arvore::ModeloVisualArvore> modeloAvl_;
    std::unique_ptr<apresentacao::arvore::ModeloVisualArvore>
        modeloRubroNegra_;
    std::unique_ptr<aplicacao::arvore::ConsultorDetalhesRegistro>
        consultorDetalhes_;
    std::unique_ptr<aplicacao::arvore::ExecutorPercursosArvore>
        executorPercursos_;
    std::unique_ptr<aplicacao::arvore::SessaoConstrucaoAvlPassoAPasso>
        sessaoAvlPassoAPasso_;
    std::unique_ptr<QTimer> temporizadorAvl_;
    TipoEstrutura tipoEstruturaAtual_{};
};
