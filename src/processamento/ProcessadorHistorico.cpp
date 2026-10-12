#include "processamento/ProcessadorHistorico.hpp"

#include <array>
#include <set>
#include <stdexcept>
#include <string>

namespace megasena::processamento {
namespace {

void validarConcursos(
    const std::vector<importacao::ConcursoImportado>& concursos)
{
    std::set<int> numerosConcursos;
    for (const auto& concurso : concursos) {
        if (concurso.numeroConcurso <= 0) {
            throw std::invalid_argument("O numero do concurso deve ser positivo.");
        }
        if (!numerosConcursos.insert(concurso.numeroConcurso).second) {
            throw std::invalid_argument(
                "O numero do concurso nao pode aparecer mais de uma vez.");
        }
        if (concurso.dezenas.size() != 6) {
            throw std::invalid_argument(
                "O concurso " + std::to_string(concurso.numeroConcurso)
                + " deve conter exatamente seis dezenas.");
        }

        std::array<bool, 61> presentes{};
        for (const int numero : concurso.dezenas) {
            if (numero < 1 || numero > 60) {
                throw std::invalid_argument(
                    "O concurso " + std::to_string(concurso.numeroConcurso)
                    + " contem dezena fora do intervalo de 1 a 60.");
            }
            if (presentes[static_cast<std::size_t>(numero)]) {
                throw std::invalid_argument(
                    "O concurso " + std::to_string(concurso.numeroConcurso)
                    + " contem dezenas repetidas.");
            }
            presentes[static_cast<std::size_t>(numero)] = true;
        }
    }
}

} // namespace

std::vector<Registro> ProcessadorHistorico::processar(
    const std::vector<importacao::ConcursoImportado>& concursos) const
{
    validarConcursos(concursos);

    std::array<int, 61> frequencias{};
    std::vector<Registro> registros;
    registros.reserve(concursos.size() * 6);

    for (const auto& concurso : concursos) {
        for (const int numero : concurso.dezenas) {
            const int frequencia = ++frequencias[static_cast<std::size_t>(numero)];
            registros.push_back({{frequencia, numero}, concurso.numeroConcurso});
        }
    }

    return registros;
}

} // namespace megasena::processamento
