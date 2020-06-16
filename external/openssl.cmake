if(NOT TARGET openssl_project)
  include(ExternalProject)
  # ----- openssl_project package -----
  ExternalProject_Add(openssl_project
                      GIT_REPOSITORY ${OPENSSL_REPOSITORY}
                      GIT_TAG ${OPENSSL_TAG}
                      GIT_PROGRESS TRUE
                      GIT_SHALLOW TRUE
                      CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}  -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD} -DCMAKE_CXX_STANDARD_REQUIRED=${CMAKE_CXX_STANDARD_REQUIRED} -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS} -DCMAKE_POSITION_INDEPENDENT_CODE=${CMAKE_POSITION_INDEPENDENT_CODE} -DBUILD_SHARED_LIBS=OFF -DWITH_APPS=OFF
                      PREFIX ${CMAKE_BINARY_DIR}/openssl_project
                      INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
                      LOG_DOWNLOAD ON
                     )
  set(OPENSSL_ROOT_DIR ${CMAKE_INSTALL_PREFIX})
  set(OPENSSL_MSVC_STATIC_RT TRUE)
  set(OPENSSL_FOUND TRUE)
  set(OPENSSL_INCLUDE_DIR "${INCLUDE_OUTPUT_DIR}/openssl")
  add_library(ssl_internal INTERFACE)
  target_link_libraries(ssl_internal INTERFACE ssl)
  target_include_directories(ssl_internal INTERFACE "${INCLUDE_OUTPUT_DIR}/openssl")
  add_library(crypto_internal INTERFACE)
  target_link_libraries(crypto_internal INTERFACE crypto)
  target_include_directories(crypto_internal INTERFACE "${INCLUDE_OUTPUT_DIR}/openssl")
  add_library(OpenSSL::SSL ALIAS ssl_internal)
  add_library(OpenSSL::Crypto ALIAS crypto_internal)
endif()

