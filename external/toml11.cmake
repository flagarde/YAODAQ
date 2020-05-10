include(ExternalProject)

if (NOT TARGET toml11)
# ----- toml11 package -----
ExternalProject_Add(
                    toml11
                    GIT_REPOSITORY ${toml11_repository}
                    GIT_TAG ${toml11_version}
                    GIT_PROGRESS TRUE
                    GIT_SHALLOW TRUE
                    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD} -DCMAKE_CXX_STANDARD_REQUIRED=${CMAKE_CXX_STANDARD_REQUIRED} -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS} -Dtoml11_BUILD_TEST=OFF -DCMAKE_INSTALL_INCLUDEDIR=${CMAKE_INSTALL_PREFIX}/include/toml  -DCMAKE_POSITION_INDEPENDENT_CODE=${CMAKE_POSITION_INDEPENDENT_CODE}
                    PREFIX ${CMAKE_BINARY_DIR}/toml11-prefix 
                    SOURCE_DIR ${CMAKE_BINARY_DIR}/toml11
                    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
                    LOG_DOWNLOAD ON
                    )
endif()
