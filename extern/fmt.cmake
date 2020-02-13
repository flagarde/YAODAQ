include(ExternalProject)

if (NOT TARGET fmt)
# ----- fmt package -----
ExternalProject_Add(
                    fmt
                    GIT_REPOSITORY ${fmt_repository}
                    GIT_TAG ${fmt_version}
                    GIT_PROGRESS TRUE
                    GIT_SHALLOW TRUE
                    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DFMT_DOC=OFF -DFMT_TEST=OFF -DFMT_FUZZ=OFF -DMASTER_PROJECT=OFF -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD} -DCMAKE_CXX_STANDARD_REQUIRED=${CMAKE_CXX_STANDARD_REQUIRED} -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS}
                    PREFIX ${CMAKE_BINARY_DIR}/fmt-prefix
                    SOURCE_DIR ${CMAKE_BINARY_DIR}/fmt
                    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
                    )
endif()

find_library(FMT_LIB fmt  "${CMAKE_INSTALL_PREFIX}/lib")
