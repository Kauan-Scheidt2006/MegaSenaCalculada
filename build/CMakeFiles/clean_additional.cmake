# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\InfraestruturaJson_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\InfraestruturaJson_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\InterfaceMegaSena_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\InterfaceMegaSena_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\MegaSenaCalculada_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\MegaSenaCalculada_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\TesteVisualizacaoArvore_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\TesteVisualizacaoArvore_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\TestesEstruturaInicial_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\TestesEstruturaInicial_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\TestesInterfaceInicial_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\TestesInterfaceInicial_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\TestesLeitorConcursosJson_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\TestesLeitorConcursosJson_autogen.dir\\ParseCache.txt"
  "InfraestruturaJson_autogen"
  "InterfaceMegaSena_autogen"
  "MegaSenaCalculada_autogen"
  "TesteVisualizacaoArvore_autogen"
  "TestesEstruturaInicial_autogen"
  "TestesInterfaceInicial_autogen"
  "TestesLeitorConcursosJson_autogen"
  )
endif()
