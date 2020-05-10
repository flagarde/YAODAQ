include(ExternalProject)

if (NOT TARGET civet)
### FIXME 
find_package(OpenSSL REQUIRED) 
set(HAVE_SSL "OFF")
if( OpenSSL_FOUND)
 set(HAVE_SSL "ON")
endif()


# ----- civet package -----
ExternalProject_Add(
                    civet DEPENDS jsroot
                    GIT_REPOSITORY ${civet_repository}
                    GIT_TAG ${civet_version}
                    GIT_PROGRESS TRUE
                    GIT_SHALLOW TRUE
                    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD} -DCMAKE_CXX_STANDARD_REQUIRED=${CMAKE_CXX_STANDARD_REQUIRED} -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS} -DCIVETWEB_BUILD_TESTING=OFF -DCIVETWEB_ENABLE_THIRD_PARTY_OUTPUT=ON -DCIVETWEB_ENABLE_SERVER_EXECUTABLE=ON -DCIVETWEB_SERVE_NO_FILES=OFF -DCIVETWEB_DISABLE_CGI=OFF -DCIVETWEB_DISABLE_CACHING=OFF -DCIVETWEB_ENABLE_CXX=ON -DCIVETWEB_ENABLE_IPV6=ON -DCIVETWEB_ENABLE_WEBSOCKETS=ON -DCIVETWEB_ENABLE_SERVER_STATS=ON -DCIVETWEB_ENABLE_MEMORY_DEBUGGING=OFF -DCIVETWEB_ENABLE_ASAN=OFF -DCIVETWEB_ENABLE_LUA=OFF -DCIVETWEB_INSTALL_EXECUTABLE=ON -DCIVETWEB_ALLOW_WARNINGS=ON -DCIVETWEB_ENABLE_DUKTAPE=OFF -DCIVETWEB_ENABLE_SSL=${HAVE_SSL} -DCIVETWEB_ENABLE_SSL_DYNAMIC_LOADING=OFF -DCIVETWEB_ENABLE_LTO=ON -DBUILD_SHARED_LIBS=OFF -DCMAKE_POSITION_INDEPENDENT_CODE=${CMAKE_POSITION_INDEPENDENT_CODE}
                    PREFIX ${CMAKE_BINARY_DIR}/civet-prefix 
                    SOURCE_DIR ${CMAKE_BINARY_DIR}/civet
                    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
                    LOG_DOWNLOAD ON
                    )
endif()
