if(NOT TARGET fmt_project)
  include(ExternalProject)
  # ----- fmt_project package -----
  externalproject_add(
    fmt_project
    GIT_REPOSITORY ${FMT_REPOSITORY}
    GIT_TAG ${FMT_VERSION}
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
    CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
               -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
               -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
               -DCMAKE_CXX_STANDARD_REQUIRED=${CMAKE_CXX_STANDARD_REQUIRED}
               -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS}
               -DCMAKE_POSITION_INDEPENDENT_CODE=${CMAKE_POSITION_INDEPENDENT_CODE}
               -DFMT_PEDANTIC=OFF
               -DFMT_WERROR=OFF
               -DFMT_DOC=OFF
               -DFMT_INSTALL=ON
               -DFMT_TEST=OFF
               -DFMT_FUZZ=OFF
               -DFMT_CUDA_TEST=OFF
               -DFMT_OS=ON
    PREFIX ${CMAKE_BINARY_DIR}/fmt_project
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    LOG_DOWNLOAD ON
    )
  add_library(fmt_internal INTERFACE)
  add_dependencies(fmt_internal fmt_project)
  # FIXME use generator expression
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(fmt_internal INTERFACE fmtd)
  else()
    target_link_libraries(fmt_internal INTERFACE fmt)
  endif()
  # FIXME uncomment target_include_directories(fmt_internal INTERFACE "${INCLUDE_OUTPUT_DIR}/fmt")
  target_compile_definitions(fmt_internal INTERFACE "FMT_LOCALE")
  add_library(fmt::fmt ALIAS fmt_internal)
endif()
