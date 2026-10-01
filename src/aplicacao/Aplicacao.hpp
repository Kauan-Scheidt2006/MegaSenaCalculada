#pragma once

#include <memory>

class Ambiente;

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

    std::unique_ptr<Ambiente> ambiente_;
};
