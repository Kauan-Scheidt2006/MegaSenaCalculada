#include "infraestrutura/json/LeitorConcursosJson.hpp"

#include "infraestrutura/json/ErroLeituraJson.hpp"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QString>

#include <cmath>
#include <limits>
#include <sstream>
#include <string_view>

namespace megasena::infraestrutura {
namespace {

QString caminhoParaQString(const std::filesystem::path& caminho)
{
    const std::u8string texto = caminho.u8string();
    return QString::fromUtf8(reinterpret_cast<const char*>(texto.data()),
                             static_cast<qsizetype>(texto.size()));
}

std::string caminhoUtf8(const std::filesystem::path& caminho)
{
    const std::u8string texto = caminho.u8string();
    return {reinterpret_cast<const char*>(texto.data()), texto.size()};
}

std::string mensagemComContexto(std::string_view motivo,
                                const std::filesystem::path& arquivo,
                                std::string_view localizacao = {})
{
    std::ostringstream mensagem;
    mensagem << motivo << " [arquivo: " << caminhoUtf8(arquivo);
    if (!localizacao.empty()) {
        mensagem << ", localização: " << localizacao;
    }
    mensagem << ']';
    return mensagem.str();
}

[[noreturn]] void falhar(std::string_view motivo,
                         const std::filesystem::path& arquivo,
                         std::string localizacao = {})
{
    throw ErroLeituraJson(
        mensagemComContexto(motivo, arquivo, localizacao),
        arquivo,
        std::move(localizacao));
}

int lerInteiro(const QJsonValue& valor,
               const std::filesystem::path& arquivo,
               const std::string& localizacao)
{
    if (!valor.isDouble()) {
        falhar("Era esperado um número inteiro", arquivo, localizacao);
    }

    const double numero = valor.toDouble();
    if (!std::isfinite(numero) || std::trunc(numero) != numero
        || numero < std::numeric_limits<int>::min()
        || numero > std::numeric_limits<int>::max()) {
        falhar("O valor não pode ser representado como inteiro", arquivo,
               localizacao);
    }

    return static_cast<int>(numero);
}

QByteArray lerConteudo(const std::filesystem::path& arquivo)
{
    QFile entrada(caminhoParaQString(arquivo));
    if (!entrada.open(QIODevice::ReadOnly)) {
        const std::string detalhe = entrada.errorString().toStdString();
        throw ErroLeituraJson(
            mensagemComContexto(
                std::string("Não foi possível abrir o arquivo JSON: ")
                    + detalhe,
                arquivo),
            arquivo);
    }

    const QByteArray conteudo = entrada.readAll();
    if (entrada.error() != QFileDevice::NoError) {
        const std::string detalhe = entrada.errorString().toStdString();
        throw ErroLeituraJson(
            mensagemComContexto(
                std::string("Não foi possível ler o arquivo JSON: ")
                    + detalhe,
                arquivo),
            arquivo);
    }

    return conteudo;
}

} // namespace

std::vector<importacao::ConcursoImportado> LeitorConcursosJson::ler(
    const std::filesystem::path& arquivo) const
{
    QJsonParseError erroAnalise;
    const QJsonDocument documento =
        QJsonDocument::fromJson(lerConteudo(arquivo), &erroAnalise);

    if (erroAnalise.error != QJsonParseError::NoError) {
        const std::string localizacao =
            "byte " + std::to_string(erroAnalise.offset);
        throw ErroLeituraJson(
            mensagemComContexto(
                std::string("JSON inválido: ")
                    + erroAnalise.errorString().toStdString(),
                arquivo, localizacao),
            arquivo, localizacao);
    }

    if (!documento.isArray()) {
        falhar("A raiz do JSON deve ser um array", arquivo, "$");
    }

    const QJsonArray itens = documento.array();
    std::vector<importacao::ConcursoImportado> concursos;
    concursos.reserve(static_cast<std::size_t>(itens.size()));

    for (qsizetype indice = 0; indice < itens.size(); ++indice) {
        const std::string base = "$[" + std::to_string(indice) + "]";
        const QJsonValue item = itens.at(indice);
        if (!item.isObject()) {
            falhar("Cada concurso deve ser um objeto", arquivo, base);
        }

        const QJsonObject objeto = item.toObject();
        if (!objeto.contains("numeroConcurso")) {
            falhar("Campo obrigatório ausente: numeroConcurso", arquivo,
                   base + ".numeroConcurso");
        }
        if (!objeto.contains("dezenas")) {
            falhar("Campo obrigatório ausente: dezenas", arquivo,
                   base + ".dezenas");
        }
        if (!objeto.contains("data")) {
            falhar("Campo obrigatório ausente: data", arquivo,
                   base + ".data");
        }

        const int numeroConcurso =
            lerInteiro(objeto.value("numeroConcurso"), arquivo,
                       base + ".numeroConcurso");

        const QJsonValue valorData = objeto.value("data");
        if (!valorData.isString()) {
            falhar("O campo data deve ser uma string", arquivo,
                   base + ".data");
        }
        const std::string data = valorData.toString().toStdString();

        const QJsonValue valorDezenas = objeto.value("dezenas");
        if (!valorDezenas.isArray()) {
            falhar("O campo dezenas deve ser um array", arquivo,
                   base + ".dezenas");
        }

        const QJsonArray dezenasJson = valorDezenas.toArray();
        std::vector<int> dezenas;
        dezenas.reserve(static_cast<std::size_t>(dezenasJson.size()));
        for (qsizetype indiceDezena = 0;
             indiceDezena < dezenasJson.size(); ++indiceDezena) {
            dezenas.push_back(lerInteiro(
                dezenasJson.at(indiceDezena), arquivo,
                base + ".dezenas[" + std::to_string(indiceDezena) + "]"));
        }

        concursos.push_back(importacao::ConcursoImportado{
            numeroConcurso,
            data,
            std::move(dezenas),
            static_cast<std::size_t>(indice)});
    }

    return concursos;
}

} // namespace megasena::infraestrutura
