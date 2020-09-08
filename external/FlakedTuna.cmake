if(NOT TARGET FlakedTuna::FlakedTuna)
  include(ExternalProject)
  # ----- FlakedTuna_project package -----
  externalproject_add(FlakedTuna_project
                      GIT_REPOSITORY ${FLAKEDTUNA_REPOSITORY}
                      GIT_TAG ${FLAKEDTUNA_VERSION}
                      GIT_PROGRESS TRUE
                      GIT_SHALLOW TRUE
                      CMAKE_ARGS -D CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                                 -D CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
                                 -D CMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
                                 -D CMAKE_CXX_STANDARD_REQUIRED=${CMAKE_CXX_STANDARD_REQUIRED}
                                 -D CMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS}
                                 -D CMAKE_POSITION_INDEPENDENT_CODE=${CMAKE_POSITION_INDEPENDENT_CODE}
                                 -D BUILD_EXAMPLES=OFF
                                 -D BUILD_TESTS=OFF
                      PREFIX ${CMAKE_BINARY_DIR}/FlakedTuna_project
                      INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
                      LOG_DOWNLOAD ON
                      #TEST_AFTER_INSTALL TRUE
                      #TEST_COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_INSTALL_PREFIX}/lib/cmake/FlakedTuna/CreatePlugins.cmake" "${CMAKECM_MODULES_DIR}/CreatePlugins.cmake"
                     )
  add_library(FlakedTuna_internal INTERFACE)
  add_dependencies(FlakedTuna_internal FlakedTuna_project)
  target_include_directories(FlakedTuna_internal INTERFACE "${INCLUDE_OUTPUT_DIR}")
  target_link_libraries(FlakedTuna_internal INTERFACE FlakedTuna)
  if(NOT EXISTS "${CMAKE_BINARY_DIR}/cmake/CreatePlugins.cmake")
    file(DOWNLOAD "https://raw.githubusercontent.com/flagarde/FlakedTuna/master/cmake/CreatePlugins.cmake" "${CMAKE_BINARY_DIR}/cmake/CreatePlugins.cmake")
  endif()
  include("${CMAKE_BINARY_DIR}/cmake/CreatePlugins.cmake")
  add_library(YAODAQ::FlakedTuna ALIAS FlakedTuna_internal)
endif()
