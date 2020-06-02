if(NOT TARGET CAENlibs)
  include(ExternalProject)
  find_package(Threads)
  set(THREADS_PREFER_PTHREAD_FLAG TRUE)
  # ----- CAENlibs package -----
  ExternalProject_Add( 
                      CAENlibs
                      GIT_REPOSITORY ${CAENlibs_repository}
                      GIT_TAG ${CAENlibs_version}
                      GIT_PROGRESS TRUE
                      GIT_SHALLOW TRUE
                      CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DCAENALL=TRUE -DINCLUDE_OUTPUT_DIRECTORY=${CMAKE_BINARY_DIR}/CAEN
                      PREFIX ${CMAKE_BINARY_DIR}/CAENlibs-prefix
                      SOURCE_DIR ${CMAKE_BINARY_DIR}/CAENlibs
                      INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
                      LOG_DOWNLOAD ON
                    )      
  add_library(CAEN INTERFACE)
  add_dependencies(CAEN CAENlibs)
  if(UNIX)
    target_compile_definitions(CAEN INTERFACE "UNIX" INTERFACE "LINUX")
  elseif(WIN32)
    target_compile_definitions(CAEN INTERFACE "_WIN32" INTERFACE "WIN32")
  endif()
  target_link_libraries(CAEN
    INTERFACE CAENVME
    INTERFACE CAENComm
    INTERFACE CAENDigitizer
    INTERFACE caenhvwrapper
    INTERFACE ${CMAKE_DL_LIBS}
    INTERFACE Threads::Threads
    INTERFACE $<$<OR:$<STREQUAL:${ARCHITECTURE},x64>,$<STREQUAL:${ARCHITECTURE},x86>>:CAENDPP>
    )
  target_include_directories(CAEN INTERFACE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/CAEN>)
  target_link_directories(CAEN INTERFACE ${LIBRARY_OUTPUT_DIR})
endif()
