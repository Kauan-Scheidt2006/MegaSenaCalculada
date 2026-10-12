cmake_minimum_required(VERSION 3.21)

project(MegaSenaCalculada LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTORCC ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.11.1/mingw_64")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

add_library(ApresentacaoArvore STATIC
    src/interface/apresentacao/arvore/DadosDetalhesRegistro.hpp
    src/interface/apresentacao/arvore/EstadoVisualizacaoArvore.hpp
    src/interface/apresentacao/arvore/IdentificadorRegistro.hpp
    src/interface/apresentacao/arvore/ModeloApresentacaoArvore.cpp
    src/interface/apresentacao/arvore/ModeloApresentacaoArvore.hpp
    src/interface/apresentacao/arvore/ModeloLigacaoArvore.hpp
    src/interface/apresentacao/arvore/ModeloNoArvore.hpp
    src/interface/apresentacao/arvore/ModeloVisualArvore.cpp
    src/interface/apresentacao/arvore/ModeloVisualArvore.hpp
)
target_include_directories(ApresentacaoArvore PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
target_link_libraries(ApresentacaoArvore PUBLIC
    Qt6::Core
)

add_library(DisposicaoArvore STATIC
    src/interface/componentes/arvore/CalculadorDisposicaoArvore.cpp
    src/interface/componentes/arvore/CalculadorDisposicaoArvore.hpp
)
target_include_directories(DisposicaoArvore PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
target_link_libraries(DisposicaoArvore PUBLIC
    ApresentacaoArvore
    Qt6::Core
)

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
    src/interface/componentes/arvore/BarraFerramentasArvore.cpp
    src/interface/componentes/arvore/BarraFerramentasArvore.hpp
    src/interface/componentes/arvore/ControleExecucaoAvl.cpp
    src/interface/componentes/arvore/ControleExecucaoAvl.hpp
    src/interface/componentes/arvore/CampoDetalhe.cpp
    src/interface/componentes/arvore/CampoDetalhe.hpp
    src/interface/componentes/arvore/ControleZoom.cpp
    src/interface/componentes/arvore/ControleZoom.hpp
    src/interface/componentes/arvore/ItemLigacaoArvore.cpp
    src/interface/componentes/arvore/ItemLigacaoArvore.hpp
    src/interface/componentes/arvore/ItemNoArvore.cpp
    src/interface/componentes/arvore/ItemNoArvore.hpp
    src/interface/componentes/arvore/MapaArvore.cpp
    src/interface/componentes/arvore/MapaArvore.hpp
    src/interface/componentes/arvore/MarcadorDezena.cpp
    src/interface/componentes/arvore/MarcadorDezena.hpp
    src/interface/componentes/arvore/GrupoDezenasConcurso.cpp
    src/interface/componentes/arvore/GrupoDezenasConcurso.hpp
    src/interface/componentes/arvore/PopoverDetalhesRegistro.cpp
    src/interface/componentes/arvore/PopoverDetalhesRegistro.hpp
    src/interface/componentes/arvore/ResultadoOperacaoArvore.cpp
    src/interface/componentes/arvore/ResultadoOperacaoArvore.hpp
    src/interface/componentes/arvore/VisualizacaoArvore.cpp
    src/interface/componentes/arvore/VisualizacaoArvore.hpp
    src/interface/apresentacao/arvore/ModeloApresentacaoArvore.hpp
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
    src/interface/paginas/arvore/ItemNavegacaoEstrutura.cpp
    src/interface/paginas/arvore/ItemNavegacaoEstrutura.hpp
    src/interface/paginas/arvore/MenuEstruturas.cpp
    src/interface/paginas/arvore/MenuEstruturas.hpp
    src/interface/paginas/arvore/PaginaArvore.cpp
    src/interface/paginas/arvore/PaginaArvore.hpp
    src/interface/paginas/arvore/PainelVisualizacaoArvore.cpp
    src/interface/paginas/arvore/PainelVisualizacaoArvore.hpp
    src/interface/paginas/arvore/TipoEstrutura.hpp
    recursos/recursos.qrc
)
target_include_directories(InterfaceMegaSena PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
target_link_libraries(InterfaceMegaSena PUBLIC
    ApresentacaoArvore
    DisposicaoArvore
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
    CasosUsoArvore
    InterfaceMegaSena
    InfraestruturaJson
    ProcessamentoHistorico
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

add_library(ProcessamentoHistorico STATIC
    src/processamento/ProcessadorHistorico.cpp
    src/processamento/ProcessadorHistorico.hpp
)
target_include_directories(ProcessamentoHistorico PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

add_library(CasosUsoArvore STATIC
    src/aplicacao/arvore/ConstrutorApresentacaoAvl.cpp
    src/aplicacao/arvore/ConstrutorApresentacaoAvl.hpp
    src/aplicacao/arvore/ConstrutorApresentacaoBst.cpp
    src/aplicacao/arvore/ConstrutorApresentacaoBst.hpp
    src/aplicacao/arvore/ConstrutorApresentacaoRubroNegra.cpp
    src/aplicacao/arvore/ConstrutorApresentacaoRubroNegra.hpp
    src/aplicacao/arvore/ConsultorDetalhesRegistro.cpp
    src/aplicacao/arvore/ConsultorDetalhesRegistro.hpp
    src/aplicacao/arvore/DetalhesConstrucaoModelo.hpp
    src/aplicacao/arvore/ExecutorPercursosArvore.cpp
    src/aplicacao/arvore/ExecutorPercursosArvore.hpp
    src/aplicacao/arvore/SessaoConstrucaoAvlPassoAPasso.cpp
    src/aplicacao/arvore/SessaoConstrucaoAvlPassoAPasso.hpp
    src/estruturas/ArvoreAvlPassoAPasso.hpp
)
target_include_directories(CasosUsoArvore PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
target_link_libraries(CasosUsoArvore PUBLIC
    ApresentacaoArvore
    EstruturasMegaSena
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

add_executable(TestesProcessamentoApresentacao
    testes/testes_processamento_apresentacao.cpp
)
target_include_directories(TestesProcessamentoApresentacao PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
target_compile_definitions(TestesProcessamentoApresentacao PRIVATE
    MEGASENA_DIRETORIO_FONTE="${CMAKE_CURRENT_SOURCE_DIR}"
)
target_link_libraries(TestesProcessamentoApresentacao PRIVATE
    ApresentacaoArvore
    CasosUsoArvore
    EstruturasMegaSena
    InfraestruturaJson
    ProcessamentoHistorico
)

add_executable(TestesModeloApresentacaoArvore
    testes/testes_modelo_apresentacao_arvore.cpp
)
target_link_libraries(TestesModeloApresentacaoArvore PRIVATE
    ApresentacaoArvore
)

add_executable(TestesDisposicaoArvore
    testes/testes_disposicao_arvore.cpp
)
target_link_libraries(TestesDisposicaoArvore PRIVATE
    DisposicaoArvore
)

enable_testing()
add_test(NAME estrutura_inicial COMMAND TestesEstruturaInicial)
add_test(NAME leitor_concursos_json COMMAND TestesLeitorConcursosJson)
add_test(NAME interface_inicial COMMAND TestesInterfaceInicial)
add_test(NAME processamento_apresentacao COMMAND TestesProcessamentoApresentacao)
add_test(NAME modelo_apresentacao_arvore COMMAND TestesModeloApresentacaoArvore)
add_test(NAME disposicao_arvore COMMAND TestesDisposicaoArvore)
set_tests_properties(interface_inicial PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
