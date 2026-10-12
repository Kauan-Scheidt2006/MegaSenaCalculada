#include "aplicacao/arvore/ConsultorDetalhesRegistro.hpp"

#include <QString>

#include <algorithm>
#include <array>
#include <map>
#include <stdexcept>

namespace aplicacao::arvore {

ConsultorDetalhesRegistro::ConsultorDetalhesRegistro(
    const std::vector<megasena::Registro>& registros,
    const std::vector<megasena::importacao::ConcursoImportado>& concursos)
{
    std::map<int, const megasena::importacao::ConcursoImportado*> concursosPorNumero;
    for (const auto& concurso : concursos) {
        if (!concursosPorNumero.emplace(concurso.numeroConcurso, &concurso).second) {
            throw std::invalid_argument(
                "Nao e possivel associar detalhes a concursos duplicados.");
        }
        if (concurso.dezenas.size() != 6) {
            throw std::invalid_argument(
                "Os detalhes exigem concursos com exatamente seis dezenas.");
        }
    }

    for (const auto& registro : registros) {
        const auto concurso = concursosPorNumero.find(registro.numeroConcurso);
        if (concurso == concursosPorNumero.end()) {
            throw std::invalid_argument(
                "O registro referencia um concurso inexistente.");
        }
        if (std::find(concurso->second->dezenas.begin(),
                      concurso->second->dezenas.end(),
                      registro.chave.numero) == concurso->second->dezenas.end()) {
            throw std::invalid_argument(
                "A dezena do registro nao pertence ao concurso associado.");
        }

        const apresentacao::arvore::IdentificadorRegistro identificador{
            registro.chave.frequenciaAcumulada, registro.chave.numero};
        std::array<int, 6> dezenas;
        std::copy_n(concurso->second->dezenas.begin(), dezenas.size(), dezenas.begin());
        const apresentacao::arvore::DadosDetalhesRegistro detalhes{
            identificador,
            concurso->second->numeroConcurso,
            QString::fromStdString(concurso->second->data),
            dezenas,
        };
        if (!detalhes_.emplace(identificador, detalhes).second) {
            throw std::invalid_argument(
                "Nao e possivel associar detalhes a uma chave duplicada.");
        }
    }
}

const apresentacao::arvore::DadosDetalhesRegistro*
ConsultorDetalhesRegistro::buscar(
    const apresentacao::arvore::IdentificadorRegistro& identificador) const noexcept
{
    const auto detalhe = detalhes_.find(identificador);
    return detalhe == detalhes_.end() ? nullptr : &detalhe->second;
}

} // namespace aplicacao::arvore
