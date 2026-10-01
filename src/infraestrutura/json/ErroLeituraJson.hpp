#pragma once

#include <filesystem>
#include <stdexcept>
#include <string>
#include <utility>

namespace megasena::infraestrutura {

class ErroLeituraJson : public std::runtime_error {
public:
    ErroLeituraJson(std::string mensagem,
                    std::filesystem::path arquivo,
                    std::string localizacao = {})
        : std::runtime_error(std::move(mensagem)),
          arquivo_(std::move(arquivo)),
          localizacao_(std::move(localizacao))
    {
    }

    [[nodiscard]] const std::filesystem::path& arquivo() const noexcept
    {
        return arquivo_;
    }

    [[nodiscard]] const std::string& localizacao() const noexcept
    {
        return localizacao_;
    }

private:
    std::filesystem::path arquivo_;
    std::string localizacao_;
};

} // namespace megasena::infraestrutura
