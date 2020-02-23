include(ExternalProject)

if (NOT TARGET elogpp)
# ----- elogpp package -----
ExternalProject_Add(
                    elogpp
                    GIT_REPOSITORY ${elogpp_repository}
                    GIT_TAG ${elogpp_version}
                    GIT_PROGRESS TRUE
                    GIT_SHALLOW TRUE
                    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD} -DCMAKE_CXX_STANDARD_REQUIRED=${CMAKE_CXX_STANDARD_REQUIRED} -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS} -DCMAKE_POSITION_INDEPENDENT_CODE=${CMAKE_POSITION_INDEPENDENT_CODE}
                    PREFIX ${CMAKE_BINARY_DIR}/elogpp-prefix
                    SOURCE_DIR ${CMAKE_BINARY_DIR}/elogpp
                    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
                    )
endif()
