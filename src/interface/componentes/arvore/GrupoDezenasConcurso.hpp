#pragma once

#include "interface/fundamentos/BaseContainer.hpp"

#include <array>

class MarcadorDezena;

class GrupoDezenasConcurso final : public BaseContainer
{
public:
    explicit GrupoDezenasConcurso(QWidget* pai = nullptr);

    void apresentar(const std::array<int, 6>& dezenas, int dezenaCorrespondente);

private:
    std::array<MarcadorDezena*, 6> marcadores_{};
};
