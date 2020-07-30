if(NOT TARGET zlib-ng_project)
  include(ExternalProject)
  set(THREADS_PREFER_PTHREAD_FLAG ON)
  find_package(Threads REQUIRED)
  # ----- zlib-ng_project package -----
  externalproject_add(
    zlib-ng_project
    GIT_REPOSITORY ${ZLIB-NG_REPOSITORY}
    GIT_TAG ${ZLIB-NG_TAG}
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
    CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
               -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
               -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
               -DCMAKE_CXX_STANDARD_REQUIRED=${CMAKE_CXX_STANDARD_REQUIRED}
               -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS}
               -DCMAKE_POSITION_INDEPENDENT_CODE=${CMAKE_POSITION_INDEPENDENT_CODE}
               -DBUILD_SHARED_LIBS=OFF
               -DZLIB_COMPAT=ON
               -DZLIB_ENABLE_TESTS=OFF
               -DWITH_GZFILEOP=OFF
               -DWITH_MSAN=ON
               -DWITH_OPTIM=ON
               -DWITH_NEW_STRATEGIES=ON
               -DWITH_NATIVE_INSTRUCTIONS=ON
               -DWITH_FUZZERS=OFF
               -DWITH_SANITIZERS=OFF
               -DWITH_MAINTAINER_WARNINGS=OFF
               -DWITH_CODE_COVERAGE=OFF
               -DZLIB_DUAL_LINK=OFF
    GIT_SUBMODULES ""
    PREFIX ${CMAKE_BINARY_DIR}/zlib-ng_project
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    LOG_DOWNLOAD ON
    )
  set(ZLIB_ROOT "${CMAKE_INSTALL_PREFIX}")
  set(ZLIB_FOUND TRUE)
  set(ZLIB_INCLUDE_DIRS "${INCLUDE_OUTPUT_DIR}")
  add_library(zlib_internal INTERFACE)
  target_link_libraries(zlib_internal INTERFACE z)
  target_include_directories(zlib_internal INTERFACE "${INCLUDE_OUTPUT_DIR}")
  add_library(ZLIB::ZLIB ALIAS zlib_internal)
endif()
