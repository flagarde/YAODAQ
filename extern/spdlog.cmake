include(ExternalProject)

if (NOT TARGET spdlog)
# ----- toml11 package -----
ExternalProject_Add(
                    spdlog
                    GIT_REPOSITORY ${spdlog_repository}
                    GIT_TAG ${spdlog_version}
                    GIT_PROGRESS TRUE
                    GIT_SHALLOW TRUE
                    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD} -DCMAKE_CXX_STANDARD_REQUIRED=${CMAKE_CXX_STANDARD_REQUIRED} -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS} -DSPDLOG_MASTER_PROJECT=ON -DSPDLOG_BUILD_EXAMPLE=OFF -DSPDLOG_BUILD_TESTS=OFF -SPDLOG_WCHAR_SUPPORT=ON -DSPDLOG_WCHAR_FILENAMES=ON
                    PREFIX ${CMAKE_BINARY_DIR}/spdlog-prefix 
                    SOURCE_DIR ${CMAKE_BINARY_DIR}/spdlog
                    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
                    )
endif()
