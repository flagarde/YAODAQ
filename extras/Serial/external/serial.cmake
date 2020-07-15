# ----- serial_project package -----
if(NOT TARGET serial_project)
  include(ExternalProject)
  ExternalProject_Add(serial_project
                      GIT_REPOSITORY ${serial_repository}
                      GIT_TAG ${serial_version}
                      GIT_PROGRESS TRUE
                      CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DCMAKE_CXX_FLAGS="-fPIC"
                      PREFIX ${CMAKE_BINARY_DIR}/serial-prefix 
                      SOURCE_DIR ${CMAKE_BINARY_DIR}/serial
                      INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
                      LOG_DOWNLOAD ON
                      UPDATE_DISCONNECTED TRUE
                     )
endif()
find_package(Threads REQUIRED)
add_library(serial_internal INTERFACE)
add_dependencies(serial_internal serial_project)
target_link_libraries(serial_internal INTERFACE serial INTERFACE Threads::Threads )
target_include_directories(serial_internal INTERFACE "${CMAKE_INSTALL_PREFIX}/include/serial")
add_library(YAODAQ::Serial ALIAS serial_internal)
