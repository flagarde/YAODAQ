## Due to a bug in spdlog we need to fetch fmt first (not using external package)
include(FetchContent)
if(NOT doctest_project_POPULATED)
  message(INFO "Compiling doctest")
  FetchContent_Declare(doctest_project
                       GIT_REPOSITORY ${DOCTEST_REPOSITORY}
                       GIT_TAG ${DOCTEST_VERSION}
                       GIT_PROGRESS TRUE
                       GIT_SHALLOW TRUE
                       LOG_DOWNLOAD ON
                      )
  FetchContent_GetProperties(doctest_project)

  FetchContent_Populate(doctest_project)
  execute_process(COMMAND cmake -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_COLOR_MAKEFILE=ON -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD} -DCMAKE_CXX_STANDARD_REQUIRED=${CMAKE_CXX_STANDARD_REQUIRED} -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS} -DCMAKE_POSITION_INDEPENDENT_CODE=${CMAKE_POSITION_INDEPENDENT_CODE} -DDOCTEST_WITH_TESTS=OFF  -DDOCTEST_NO_INSTALL=OFF -S "${CMAKE_BINARY_DIR}/_deps/doctest_project-src" -B "${CMAKE_BINARY_DIR}/_deps/doctest_project-build" -G ${CMAKE_GENERATOR})
  execute_process(COMMAND cmake --build "${CMAKE_BINARY_DIR}/_deps/doctest_project-build" )
  execute_process(COMMAND cmake --install "${CMAKE_BINARY_DIR}/_deps/doctest_project-build")
  message(INFO "doctest installed !")
endif()

find_package(doctest REQUIRED)
add_library(doctest INTERFACE)
add_dependencies(doctest doctest_project)
target_link_libraries(doctest INTERFACE doctest::doctest)
target_include_directories(doctest INTERFACE "${INCLUDE_OUTPUT_DIR}/doctest")

file(WRITE "${CMAKE_BINARY_DIR}/generated/Main.cpp" "#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include \"doctest.h\"")
add_library(DoctestMain "${CMAKE_BINARY_DIR}/generated/Main.cpp")
add_dependencies(DoctestMain doctest)
target_link_libraries(DoctestMain PUBLIC doctest)
