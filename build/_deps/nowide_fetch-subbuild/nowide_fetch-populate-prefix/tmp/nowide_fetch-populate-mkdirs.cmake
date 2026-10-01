# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "C:/Users/Kauan/Documents/ChatGPT/Mega Sena Calculada/MegaSenaCalculada/build/_deps/nowide_fetch-src")
  file(MAKE_DIRECTORY "C:/Users/Kauan/Documents/ChatGPT/Mega Sena Calculada/MegaSenaCalculada/build/_deps/nowide_fetch-src")
endif()
file(MAKE_DIRECTORY
  "C:/Users/Kauan/Documents/ChatGPT/Mega Sena Calculada/MegaSenaCalculada/build/_deps/nowide_fetch-build"
  "C:/Users/Kauan/Documents/ChatGPT/Mega Sena Calculada/MegaSenaCalculada/build/_deps/nowide_fetch-subbuild/nowide_fetch-populate-prefix"
  "C:/Users/Kauan/Documents/ChatGPT/Mega Sena Calculada/MegaSenaCalculada/build/_deps/nowide_fetch-subbuild/nowide_fetch-populate-prefix/tmp"
  "C:/Users/Kauan/Documents/ChatGPT/Mega Sena Calculada/MegaSenaCalculada/build/_deps/nowide_fetch-subbuild/nowide_fetch-populate-prefix/src/nowide_fetch-populate-stamp"
  "C:/Users/Kauan/Documents/ChatGPT/Mega Sena Calculada/MegaSenaCalculada/build/_deps/nowide_fetch-subbuild/nowide_fetch-populate-prefix/src"
  "C:/Users/Kauan/Documents/ChatGPT/Mega Sena Calculada/MegaSenaCalculada/build/_deps/nowide_fetch-subbuild/nowide_fetch-populate-prefix/src/nowide_fetch-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/Kauan/Documents/ChatGPT/Mega Sena Calculada/MegaSenaCalculada/build/_deps/nowide_fetch-subbuild/nowide_fetch-populate-prefix/src/nowide_fetch-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/Kauan/Documents/ChatGPT/Mega Sena Calculada/MegaSenaCalculada/build/_deps/nowide_fetch-subbuild/nowide_fetch-populate-prefix/src/nowide_fetch-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
