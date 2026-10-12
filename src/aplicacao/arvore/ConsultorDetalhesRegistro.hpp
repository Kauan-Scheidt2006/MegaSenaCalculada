#pragma once

#include "dominio/Registro.hpp"
#include "importacao/ConcursoImportado.hpp"
#include "interface/apresentacao/arvore/DadosDetalhesRegistro.hpp"

#include <map>
#include <vector>

namespace aplicacao::arvore {

class ConsultorDetalhesRegistro final {
public:
    ConsultorDetalhesRegistro(
        const std::vector<megasena::Registro>& registros,
        const std::vector<megasena::importacao::ConcursoImportado>& concursos);

    [[nodiscard]] const apresentacao::arvore::DadosDetalhesRegistro* buscar(
        const apresentacao::arvore::IdentificadorRegistro& identificador) const noexcept;

private:
    std::map<apresentacao::arvore::IdentificadorRegistro,
             apresentacao::arvore::DadosDetalhesRegistro> detalhes_;
};

} // namespace aplicacao::arvore
