if(NOT TARGET IXWebSocket_project)
  include(ExternalProject)
  include(OpenSSL)
  include(zlib-ng)
  set(THREADS_PREFER_PTHREAD_FLAG ON)
  find_package(Threads REQUIRED)
  # ----- IXWebSocket_project package -----
  externalproject_add(
    IXWebSocket_project
    DEPENDS OpenSSL::SSL
    DEPENDS zlib-ng_project
    GIT_REPOSITORY ${IXWEBSOCKET_REPOSITORY}
    GIT_TAG ${IXWEBSOCKET_TAG}
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
    CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
               -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
               -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
               -DCMAKE_CXX_STANDARD_REQUIRED=${CMAKE_CXX_STANDARD_REQUIRED}
               -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS}
               -DUSE_TLS=TRUE
               -DUSE_WS=FALSE
               -DCMAKE_POSITION_INDEPENDENT_CODE=${CMAKE_POSITION_INDEPENDENT_CODE}
               -DUSE_OPEN_SSL=TRUE
               -DZLIB_ROOT=${ZLIB_ROOT}
               -DOPENSSL_ROOT_DIR=${CMAKE_INSTALL_PREFIX}
    GIT_SUBMODULES ""
    PREFIX ${CMAKE_BINARY_DIR}/IXWebSocket_project
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    LOG_DOWNLOAD ON
    )
  add_library(IXWebSocket INTERFACE)
  add_dependencies(IXWebSocket IXWebSocket_project)
  target_link_libraries(
    IXWebSocket
    INTERFACE ixwebsocket
    INTERFACE OpenSSL::Crypto
    INTERFACE OpenSSL::SSL
    INTERFACE Threads::Threads
    INTERFACE ZLIB::ZLIB
    INTERFACE ${CMAKE_DL_LIBS}
    )
  target_include_directories(IXWebSocket INTERFACE "${INCLUDE_OUTPUT_DIR}/ixwebsocket")
endif()
