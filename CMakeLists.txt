cmake_minimum_required(VERSION 3.21)

project(MegaSenaCalculada LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTORCC ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.11.1/mingw_64")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

add_library(InterfaceMegaSena STATIC
    src/interface/ambiente/Ambiente.cpp
    src/interface/ambiente/Ambiente.hpp
    src/interface/ambiente/AreaPaginas.cpp
    src/interface/ambiente/AreaPaginas.hpp
    src/interface/ambiente/BarraEstado.cpp
    src/interface/ambiente/BarraEstado.hpp
    src/interface/ambiente/EstruturaAmbiente.cpp
    src/interface/ambiente/EstruturaAmbiente.hpp
    src/interface/componentes/basicos/Botao.cpp
    src/interface/componentes/basicos/Botao.hpp
    src/interface/componentes/basicos/BotaoIcone.cpp
    src/interface/componentes/basicos/BotaoIcone.hpp
    src/interface/componentes/basicos/BotaoNavegacao.cpp
    src/interface/componentes/basicos/BotaoNavegacao.hpp
    src/interface/componentes/basicos/Imagem.cpp
    src/interface/componentes/basicos/Imagem.hpp
    src/interface/componentes/basicos/IndicadorProgresso.cpp
    src/interface/componentes/basicos/IndicadorProgresso.hpp
    src/interface/componentes/basicos/Subtitulo.cpp
    src/interface/componentes/basicos/Subtitulo.hpp
    src/interface/componentes/basicos/Texto.cpp
    src/interface/componentes/basicos/Texto.hpp
    src/interface/componentes/basicos/Titulo.cpp
    src/interface/componentes/basicos/Titulo.hpp
    src/interface/estilos/EstiloAplicacao.cpp
    src/interface/estilos/EstiloAplicacao.hpp
    src/interface/fundamentos/Barra.cpp
    src/interface/fundamentos/Barra.hpp
    src/interface/fundamentos/BaseContainer.cpp
    src/interface/fundamentos/BaseContainer.hpp
    src/interface/fundamentos/BasePagina.cpp
    src/interface/fundamentos/BasePagina.hpp
    src/interface/fundamentos/ConfiguradorComponente.cpp
    src/interface/fundamentos/ConfiguradorComponente.hpp
    src/interface/fundamentos/HospedagemRaiz.cpp
    src/interface/fundamentos/HospedagemRaiz.hpp
    src/interface/fundamentos/Painel.cpp
    src/interface/fundamentos/Painel.hpp
    src/interface/parametros/ParametrosComponente.hpp
    src/interface/parametros/ParametrosContainer.hpp
    src/interface/parametros/TipoDisposicao.hpp
    src/interface/paginas/arvore/CabecalhoPaginaArvore.cpp
    src/interface/paginas/arvore/CabecalhoPaginaArvore.hpp
    src/interface/paginas/arvore/ConteudoPaginaArvore.cpp
    src/interface/paginas/arvore/ConteudoPaginaArvore.hpp
    src/interface/paginas/arvore/PaginaArvore.cpp
    src/interface/paginas/arvore/PaginaArvore.hpp
    src/interface/paginas/arvore/PainelVisualizacaoArvore.cpp
    src/interface/paginas/arvore/PainelVisualizacaoArvore.hpp
    recursos/recursos.qrc
)
target_include_directories(InterfaceMegaSena PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
target_link_libraries(InterfaceMegaSena PUBLIC
    Qt6::Core
    Qt6::Widgets
)

qt_add_executable(MegaSenaCalculada WIN32
    principal.cpp
    src/aplicacao/Aplicacao.cpp
    src/aplicacao/Aplicacao.hpp
)
target_include_directories(MegaSenaCalculada PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
target_link_libraries(MegaSenaCalculada PRIVATE
    InterfaceMegaSena
)

add_library(EstruturasMegaSena INTERFACE)
target_include_directories(EstruturasMegaSena INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

add_library(InfraestruturaJson
    src/infraestrutura/json/LeitorConcursosJson.cpp
)
target_include_directories(InfraestruturaJson PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
target_link_libraries(InfraestruturaJson PRIVATE
    Qt6::Core
)

qt_add_executable(TesteVisualizacaoArvore WIN32
    testeApresentacao.cpp
    ArvoreVisualizacao.cpp
    ArvoreVisualizacao.h
    concursosTestes.h
)

target_link_libraries(TesteVisualizacaoArvore PRIVATE
    EstruturasMegaSena
    Qt6::Widgets
)

add_executable(TestesEstruturaInicial
    testes/testes_estrutura_inicial.cpp
)

target_link_libraries(TestesEstruturaInicial PRIVATE
    EstruturasMegaSena
)

add_executable(TestesLeitorConcursosJson
    testes/testes_leitor_concursos_json.cpp
)
target_link_libraries(TestesLeitorConcursosJson PRIVATE
    InfraestruturaJson
)

add_executable(TestesInterfaceInicial
    testes/testes_interface_inicial.cpp
)
target_link_libraries(TestesInterfaceInicial PRIVATE
    InterfaceMegaSena
)

enable_testing()
add_test(NAME estrutura_inicial COMMAND TestesEstruturaInicial)
add_test(NAME leitor_concursos_json COMMAND TestesLeitorConcursosJson)
add_test(NAME interface_inicial COMMAND TestesInterfaceInicial)
set_tests_properties(interface_inicial PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
