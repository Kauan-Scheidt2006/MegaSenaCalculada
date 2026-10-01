#pragma once

#include "dominio/Registro.hpp"

#include <array>

namespace testes_apresentacao {

inline constexpr std::array<megasena::Registro, 24> registrosConcursosTeste{{
    {{1, 1}, 1001},
    {{1, 12}, 1001},
    {{1, 20}, 1001},
    {{1, 25}, 1001},
    {{1, 30}, 1001},
    {{1, 40}, 1001}, //1 concurso
    {{1, 3}, 1002},
    {{2, 20}, 1002},
    {{1, 38}, 1002},
    {{2, 40}, 1002},
    {{1, 47}, 1002},
    {{1, 55}, 1002}, //2 concurso
    {{1, 5}, 1003},
    {{1, 10}, 1003},
    {{1, 15}, 1003},
    {{3, 20}, 1003},
    {{2, 25}, 1003},
    {{2, 30}, 1003}, //3 concurso
    {{1, 7}, 1004},
    {{1, 14}, 1004},
    {{1, 21}, 1004},
    {{1, 28}, 1004},
    {{1, 35}, 1004},
    {{1, 42}, 1004} //4 concurso

}};

} // namespace testes_apresentacao
