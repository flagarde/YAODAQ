include(ExternalProject)

if (NOT TARGET jsroot)
# ----- jsroot package -----
ExternalProject_Add(
                    jsroot
                    GIT_REPOSITORY ${jsroot_repository}
                    GIT_TAG ${jsroot_version}
                    GIT_PROGRESS TRUE
                    GIT_SHALLOW TRUE
                    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD} -DCMAKE_CXX_STANDARD_REQUIRED=${CMAKE_CXX_STANDARD_REQUIRED} -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS} -DCMAKE_POSITION_INDEPENDENT_CODE=${CMAKE_POSITION_INDEPENDENT_CODE}
                    GIT_SUBMODULES ""
                    CONFIGURE_COMMAND ""
                    BUILD_COMMAND ""
                    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_BINARY_DIR}/jsroot
                ${CMAKE_INSTALL_PREFIX}/www/jsroot
                    PREFIX ${CMAKE_BINARY_DIR}/jsroot-prefix
                    SOURCE_DIR ${CMAKE_BINARY_DIR}/jsroot
                    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
                    LOG_DOWNLOAD ON
                    )
endif()
