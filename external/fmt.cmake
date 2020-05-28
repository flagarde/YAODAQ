## Due to a bug in spdlog we need to fetch fmt first (not using external package)
include(FetchContent)
if(NOT fmt_project_POPULATED)
  message(INFO "Compiling fmt")
  FetchContent_Declare(fmt_project
                       GIT_REPOSITORY ${FMT_REPOSITORY}
                       GIT_TAG ${FMT_VERSION}
                       GIT_PROGRESS TRUE
                       GIT_SHALLOW TRUE
                       LOG_DOWNLOAD ON
                      )
  FetchContent_GetProperties(fmt_project)

  FetchContent_Populate(fmt_project)
  execute_process(COMMAND cmake -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_COLOR_MAKEFILE=ON -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD} -DCMAKE_CXX_STANDARD_REQUIRED=${CMAKE_CXX_STANDARD_REQUIRED} -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS} -DCMAKE_POSITION_INDEPENDENT_CODE=${CMAKE_POSITION_INDEPENDENT_CODE} -DFMT_PEDANTIC=OFF -DFMT_WERROR=OFF -DFMT_DOC=OFF -DFMT_INSTALL=ON -DFMT_TEST=OFF -DFMT_FUZZ=OFF -DFMT_CUDA_TEST=OFF -DFMT_OS=ON -S "${CMAKE_BINARY_DIR}/_deps/fmt_project-src" -B "${CMAKE_BINARY_DIR}/_deps/fmt_project-build" -G ${CMAKE_GENERATOR})
  execute_process(COMMAND cmake --build "${CMAKE_BINARY_DIR}/_deps/fmt_project-build" )
  execute_process(COMMAND cmake --install "${CMAKE_BINARY_DIR}/_deps/fmt_project-build")
  message(INFO "fmt compiled !")
endif()

find_package(fmt REQUIRED)
