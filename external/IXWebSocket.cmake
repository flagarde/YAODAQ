include(ExternalProject)

if (NOT TARGET IXWebSocket)
# ----- IXWebSocket package -----
ExternalProject_Add(
                    IXWebSocket
                    GIT_REPOSITORY ${IXWebSocket_repository}
                    GIT_TAG ${IXWebSocket_version}
                    GIT_PROGRESS TRUE
                    GIT_SHALLOW TRUE
                    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD} -DCMAKE_CXX_STANDARD_REQUIRED=${CMAKE_CXX_STANDARD_REQUIRED} -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS} -DUSE_TLS=TRUE -DUSE_WS=FALSE -DUSE_MBED_TLS=TRUE -DUSE_VENDORED_THIRD_PARTY=TRUE -DCMAKE_POSITION_INDEPENDENT_CODE=${CMAKE_POSITION_INDEPENDENT_CODE}
                    GIT_SUBMODULES ""
                    PREFIX ${CMAKE_BINARY_DIR}/IXWebSocket-prefix
                    SOURCE_DIR ${CMAKE_BINARY_DIR}/IXWebSocket
                    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
                    )
endif()
