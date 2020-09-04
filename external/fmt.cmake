if(NOT TARGET fmt::fmt)
  include(ExternalProject)
  # ----- fmt_project package -----
  externalproject_add(fmt_project
                      GIT_REPOSITORY ${FMT_REPOSITORY}
                      GIT_TAG ${FMT_VERSION}
                      GIT_PROGRESS TRUE
                      GIT_SHALLOW TRUE
                      CMAKE_ARGS -D CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                                 -D CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
                                 -D CMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
                                 -D CMAKE_CXX_STANDARD_REQUIRED=${CMAKE_CXX_STANDARD_REQUIRED}
                                 -D CMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS}
                                 -D CMAKE_POSITION_INDEPENDENT_CODE=${CMAKE_POSITION_INDEPENDENT_CODE}
                                 -D FMT_PEDANTIC=OFF
                                 -D FMT_WERROR=OFF
                                 -D FMT_DOC=OFF
                                 -D FMT_INSTALL=ON
                                 -D FMT_TEST=OFF
                                 -D FMT_FUZZ=OFF
                                 -D FMT_CUDA_TEST=OFF
                                 -D FMT_OS=ON
                      PREFIX ${CMAKE_BINARY_DIR}/fmt_project
                      INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
                      LOG_DOWNLOAD ON
                     )
  add_library(fmt_internal INTERFACE)
  add_dependencies(fmt_internal fmt_project)
  target_link_libraries(fmt_internal INTERFACE "fmt$<$<CONFIG:Debug>:d>")
  target_include_directories(fmt_internal INTERFACE "${INCLUDE_OUTPUT_DIR}")
  include(CheckSymbolExists)
  set(strtod_l_headers stdlib.h)
  if(APPLE)
    set(strtod_l_headers ${strtod_l_headers} xlocale.h)
  endif()
  if(WIN32)
    check_symbol_exists(_strtod_l "${strtod_l_headers}" HAVE_STRTOD_L)
  else()
    check_symbol_exists(strtod_l "${strtod_l_headers}" HAVE_STRTOD_L)
  endif()
  if(HAVE_STRTOD_L)
    target_compile_definitions(fmt_internal INTERFACE FMT_LOCALE)
  endif()
  add_library(fmt::fmt ALIAS fmt_internal)
endif()
