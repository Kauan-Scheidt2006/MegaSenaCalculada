#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "OpenXLSX::static" for configuration "Debug"
set_property(TARGET OpenXLSX::static APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(OpenXLSX::static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/OpenXLSX/libpugixml.a"
  )

list(APPEND _cmake_import_check_targets OpenXLSX::static )
list(APPEND _cmake_import_check_files_for_OpenXLSX::static "${_IMPORT_PREFIX}/lib/OpenXLSX/libpugixml.a" )

# Import target "OpenXLSX::miniz" for configuration "Debug"
set_property(TARGET OpenXLSX::miniz APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(OpenXLSX::miniz PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/OpenXLSX/libminiz.a"
  )

list(APPEND _cmake_import_check_targets OpenXLSX::miniz )
list(APPEND _cmake_import_check_files_for_OpenXLSX::miniz "${_IMPORT_PREFIX}/lib/OpenXLSX/libminiz.a" )

# Import target "OpenXLSX::nowide" for configuration "Debug"
set_property(TARGET OpenXLSX::nowide APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(OpenXLSX::nowide PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/OpenXLSX/libnowide.a"
  )

list(APPEND _cmake_import_check_targets OpenXLSX::nowide )
list(APPEND _cmake_import_check_files_for_OpenXLSX::nowide "${_IMPORT_PREFIX}/lib/OpenXLSX/libnowide.a" )

# Import target "OpenXLSX::OpenXLSX" for configuration "Debug"
set_property(TARGET OpenXLSX::OpenXLSX APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(OpenXLSX::OpenXLSX PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libOpenXLSXd.a"
  )

list(APPEND _cmake_import_check_targets OpenXLSX::OpenXLSX )
list(APPEND _cmake_import_check_files_for_OpenXLSX::OpenXLSX "${_IMPORT_PREFIX}/lib/libOpenXLSXd.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
