include_guard(GLOBAL)

include(Messages)

find_package(LCIO QUIET)
if(NOT LCIO_FOUND)
  if(NOT LCIO_URL)
    set(LCIO_URL "https://github.com/iLCSoft/LCIO.git")
  endif()
  if(NOT LCIO_TAG)
    set(LCIO_TAG "master")
  endif()
  message(WARN "Could NOT find LCIO (missing: LCIO_DIR) !")
  message(INFO "Downloading form ${LCIO_URL}, Tag : ${LCIO_TAG}")
  find_package(Git REQUIRED)
  include(ExternalProject)
  # ----- LCIO_project package -----
  externalproject_add(
    LCIO_project
    GIT_REPOSITORY ${LCIO_URL}
    GIT_TAG ${LCIO_TAG}
    TIMEOUT 10
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
    LOG_DOWNLOAD ON
    UPDATE_DISCONNECTED TRUE
    CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
               -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
               -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
               -DCMAKE_CXX_STANDARD_REQUIRED=${CMAKE_CXX_STANDARD_REQUIRED}
               -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS}
               -DCMAKE_POSITION_INDEPENDENT_CODE=${CMAKE_POSITION_INDEPENDENT_CODE}
               -DINSTALL_JAR=OFF
               -DLCIO_GENERATE_HEADERS=OFF
               -DBUILD_ROOTDICT=OFF
               -DBUILD_LCIO_EXAMPLES=OFF
               -DBUILD_F77_TESTJOBS=OFF
               -DINSTALL_DOC=OFF
               -DBUILD_TESTING=OFF
               -DBUILD_FORTRAN=OFF
               -DINSTALL_PYTHON=OFF
    PREFIX ${CMAKE_BINARY_DIR}/LCIO_project
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    )
  add_library(LCIO_internal INTERFACE)
  add_dependencies(LCIO_internal LCIO_project)
  target_include_directories(
    LCIO_internal
    INTERFACE "${CMAKE_INSTALL_PREFIX}/include"
    INTERFACE "${CMAKE_INSTALL_PREFIX}/include/DATA"
    INTERFACE "${CMAKE_INSTALL_PREFIX}/include/EVENT"
    INTERFACE "${CMAKE_INSTALL_PREFIX}/include/IMPL"
    INTERFACE "${CMAKE_INSTALL_PREFIX}/include/IO"
    INTERFACE "${CMAKE_INSTALL_PREFIX}/include/IOIMPL"
    INTERFACE "${CMAKE_INSTALL_PREFIX}/include/LCIO"
    INTERFACE "${CMAKE_INSTALL_PREFIX}/include/MT"
    INTERFACE "${CMAKE_INSTALL_PREFIX}/include/rootDict"
    INTERFACE "${CMAKE_INSTALL_PREFIX}/include/SIO"
    INTERFACE "${CMAKE_INSTALL_PREFIX}/include/UTIL"
    )
  target_link_libraries(LCIO_internal INTERFACE lcio INTERFACE sio)
  add_library(LCIO::LCIO ALIAS LCIO_internal)
else()
  find_package(LCIO REQUIRED)
  add_library(LCIO_internal INTERFACE)
  target_link_libraries(LCIO_internal INTERFACE ${LCIO_LIBRARIES})
  target_include_directories(
    LCIO_internal
    INTERFACE "${LCIO_INCLUDE_DIRS}"
    INTERFACE "${LCIO_INCLUDE_DIRS}/DATA"
    INTERFACE "${LCIO_INCLUDE_DIRS}/EVENT"
    INTERFACE "${LCIO_INCLUDE_DIRS}/IMPL"
    INTERFACE "${LCIO_INCLUDE_DIRS}/IO"
    INTERFACE "${LCIO_INCLUDE_DIRS}/IOIMPL"
    INTERFACE "${LCIO_INCLUDE_DIRS}/LCIO"
    INTERFACE "${LCIO_INCLUDE_DIRS}/MT"
    INTERFACE "${LCIO_INCLUDE_DIRS}/rootDict"
    INTERFACE "${LCIO_INCLUDE_DIRS}/SIO"
    INTERFACE "${LCIO_INCLUDE_DIRS}/UTIL"
    )
  add_library(LCIO::LCIO ALIAS LCIO_internal)
endif()
