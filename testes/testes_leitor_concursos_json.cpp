#include "infraestrutura/json/ErroLeituraJson.hpp"
#include "infraestrutura/json/LeitorConcursosJson.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>
#include <system_error>
#include <utility>
#include <vector>

namespace {

using megasena::infraestrutura::ErroLeituraJson;
using megasena::infraestrutura::LeitorConcursosJson;

class ArquivoTemporario {
public:
    ArquivoTemporario(std::string nome, std::string_view conteudo)
        : caminho_(std::filesystem::temp_directory_path() / std::move(nome))
    {
        std::ofstream saida(caminho_, std::ios::binary | std::ios::trunc);
        saida << conteudo;
        assert(saida.good());
    }

    ~ArquivoTemporario()
    {
        std::error_code erro;
        std::filesystem::remove(caminho_, erro);
    }

    [[nodiscard]] const std::filesystem::path& caminho() const noexcept
    {
        return caminho_;
    }

private:
    std::filesystem::path caminho_;
};

void testarLeituraDosConcursos()
{
    const ArquivoTemporario arquivo(
        "megasena_concursos_validos.json",
        R"([
            {"numeroConcurso": 2, "data": "18/03/1996", "dezenas": [60, 5, 14, 22, 33, 41]},
            {"numeroConcurso": 1, "data": "11/03/1996", "dezenas": [4, 5, 30, 33, 41, 52]}
        ])");

    const auto concursos = LeitorConcursosJson{}.ler(arquivo.caminho());

    assert(concursos.size() == 2);
    assert(concursos[0].numeroConcurso == 2);
    assert(concursos[0].data == "18/03/1996");
    assert((concursos[0].dezenas == std::vector<int>{60, 5, 14, 22, 33, 41}));
    assert(concursos[0].indiceOrigem == 0);
    assert(concursos[1].numeroConcurso == 1);
    assert(concursos[1].indiceOrigem == 1);
}

void testarJsonMalformado()
{
    const ArquivoTemporario arquivo(
        "megasena_concursos_malformados.json",
        R"([{"numeroConcurso": 1, "data": "11/03/1996", "dezenas": [1, 2])");

    try {
        static_cast<void>(LeitorConcursosJson{}.ler(arquivo.caminho()));
        assert(false);
    }
    catch (const ErroLeituraJson& erro) {
        assert(erro.arquivo() == arquivo.caminho());
        assert(erro.localizacao().starts_with("byte "));
    }
}

void testarCampoAusente()
{
    const ArquivoTemporario arquivo(
        "megasena_concurso_sem_dezenas.json",
        R"([{"numeroConcurso": 1, "data": "11/03/1996"}])");

    try {
        static_cast<void>(LeitorConcursosJson{}.ler(arquivo.caminho()));
        assert(false);
    }
    catch (const ErroLeituraJson& erro) {
        assert(erro.localizacao() == "$[0].dezenas");
    }
}

void testarTipoInvalidoComLocalizacao()
{
    const ArquivoTemporario arquivo(
        "megasena_concurso_tipo_invalido.json",
        R"([{"numeroConcurso": 1, "data": "11/03/1996", "dezenas": [4, "cinco"]}])");

    try {
        static_cast<void>(LeitorConcursosJson{}.ler(arquivo.caminho()));
        assert(false);
    }
    catch (const ErroLeituraJson& erro) {
        assert(erro.localizacao() == "$[0].dezenas[1]");
    }
}

void testarArquivoInexistente()
{
    const auto arquivo = std::filesystem::temp_directory_path()
        / "megasena_concursos_inexistentes.json";

    try {
        static_cast<void>(LeitorConcursosJson{}.ler(arquivo));
        assert(false);
    }
    catch (const ErroLeituraJson& erro) {
        assert(erro.arquivo() == arquivo);
        assert(erro.localizacao().empty());
    }
}

} // namespace

int main()
{
    testarLeituraDosConcursos();
    testarJsonMalformado();
    testarCampoAusente();
    testarTipoInvalidoComLocalizacao();
    testarArquivoInexistente();
    return 0;
}
