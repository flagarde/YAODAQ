include(ExternalProject)

if (NOT TARGET toml11)
# ----- toml11 package -----
ExternalProject_Add(
                    toml11
                    GIT_REPOSITORY ${toml11_repository}
                    GIT_TAG ${toml11_version}
                    GIT_PROGRESS TRUE
                    GIT_SHALLOW TRUE
                    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD} -DCMAKE_CXX_STANDARD_REQUIRED=${CMAKE_CXX_STANDARD_REQUIRED} -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS} -DCMAKE_PLATFORM_INDEPENDENT_CODE=${CMAKE_PLATFORM_INDEPENDENT_CODE} -Dtoml11_BUILD_TEST=OFF -DCMAKE_INSTALL_INCLUDEDIR=${CMAKE_INSTALL_PREFIX}/include 
                    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/toml11-prefix 
                    SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/toml11
                    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
                    )
endif()
