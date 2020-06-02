###################################
#   CMakeLists.txt to build DAQ   #
#   Author LAGARDE Francois       #
###################################
cmake_minimum_required(VERSION 3.9...3.17.2 FATAL_ERROR)

project(YAODAQ VERSION "0.0.1.0" DESCRIPTION "YAODAQ for RPClab" HOMEPAGE_URL "https://github.com/RPClab/YAODAQ" LANGUAGES CXX)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/bin" CACHE PATH "Default Install Prefix" FORCE)
  message(STATUS "Install Prefix set to : ${CMAKE_INSTALL_PREFIX}")
endif()

# Ask CMake to output a compile_commands.json file for use with things like Vim YCM.
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
set(CMAKE_LINK_DEPENDS_NO_SHARED TRUE)

if(CMAKE_SYSTEM_NAME STREQUAL Linux)
  include(GNUInstallDirs)
  set(LIBRARY_OUTPUT_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
  set(RUNTIME_OUTPUT_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}")
  set(ARCHIVE_OUTPUT_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
  set(INCLUDE_OUTPUT_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}")
  set(CMAKE_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/cmake")
else()
  set(LIBRARY_OUTPUT_DIR "${CMAKE_INSTALL_PREFIX}/lib")
  set(RUNTIME_OUTPUT_DIR "${CMAKE_INSTALL_PREFIX}")
  set(ARCHIVE_OUTPUT_DIR "${CMAKE_INSTALL_PREFIX}/lib")
  set(INCLUDE_OUTPUT_DIR "${CMAKE_INSTALL_PREFIX}/include")
  set(CMAKE_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}/lib/cmake")
endif()

set(CMAKE_MACOSX_RPATH TRUE)
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "${LIBRARY_OUTPUT_DIR}")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
link_directories("${LIBRARY_OUTPUT_DIR}")

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_CURRENT_SOURCE_DIR}/cmake" "${CMAKE_CURRENT_SOURCE_DIR}/cmake/scripts" "${CMAKE_CURRENT_SOURCE_DIR}/external" "${CMAKE_CURRENT_SOURCE_DIR}" )
set(CMAKE_PREFIX_PATH "${CMAKE_OUTPUT_DIRECTORY}")

include(Colors)

include(Settings)

## Install doctest to allow testing exectuables and test inside executables
include(CTest)
include(doctest)
include(${CMAKE_OUTPUT_DIRECTORY}/doctest/doctest.cmake)

## If we want to have test in the executables 
if(ENABLE_DOCTESTS)
  add_dependencies(doctest)
  add_library(doctest)
  add_definitions(-DENABLE_DOCTEST_IN_LIBRARY)
endif()

include(ProcessorCount)
ProcessorCount(N)
if(NOT N EQUAL 0)
  set(CTEST_BUILD_FLAGS -j${N})
endif()

file(DOWNLOAD "${PMM_PATH}" "${CMAKE_BINARY_DIR}/pmm.cmake" SHOW_PROGRESS EXPECTED_HASH SHA256=${PMM_SHA256})
include("${CMAKE_BINARY_DIR}/pmm.cmake")

if(WIN32)
  ##FIXME
  set(CMAKE_SHARED_LINKER_FLAGS_COVERAGE "" CACHE STRING "Linker")
  set(CMAKE_EXE_LINKER_FLAGS_COVERAGE "" CACHE STRING "Linker")
  set(CMAKE_EXE_LINKER_FLAGS_PROFILE "" CACHE STRING "Linker")
  set(CMAKE_SHARED_LINKER_FLAGS_PROFILE "" CACHE STRING "Linker")
  pmm(DEBUG CONAN)
elseif(APPLE)
  pmm(DEBUG CONAN)
else()
  pmm(DEBUG CONAN SETTINGS "compiler.libcxx=libstdc++11;compiler.version=9.2")
endif()
include(${PROJECT_BINARY_DIR}/conanbuildinfo.cmake)

include(GenerateReadMe)
generate_readme()





include(Architectures)

# set the C++ standard to C++ 17
include(CXXStandards)
cxx_17()

include(Formatting)
file(
  GLOB_RECURSE
  ALL_CODE_FILES
  ${PROJECT_SOURCE_DIR}/libs/*.[ch]pp
  ${PROJECT_SOURCE_DIR}/libs/*.[ch]
  ${PROJECT_SOURCE_DIR}/apps/*.[ch]pp
  ${PROJECT_SOURCE_DIR}/apps/*.[ch]
  ${PROJECT_SOURCE_DIR}/examples/*.[ch]pp
  ${PROJECT_SOURCE_DIR}/examples/*.[ch]
  ${PROJECT_SOURCE_DIR}/tests/*.[ch]pp
  ${PROJECT_SOURCE_DIR}/tests/*.[ch]
  )

clang_format(clang-format ${ALL_CODE_FILES})

file(GLOB_RECURSE CMAKE_FILES CMakeLists.txt ${PROJECT_SOURCE_DIR}/cmake ${PROJECT_SOURCE_DIR}/extern ${PROJECT_SOURCE_DIR}/Options.cmake)

cmake_format(cmake-format ${CMAKE_FILES})

# guard against in-source builds
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif()

if(CMAKE_CONFIGURATION_TYPES)
  if(NOT "Coverage" IN_LIST CMAKE_CONFIGURATION_TYPES)
    list(APPEND CMAKE_CONFIGURATION_TYPES Coverage)
  endif()
  if(NOT "Profile" IN_LIST CMAKE_CONFIGURATION_TYPES)
    list(APPEND CMAKE_CONFIGURATION_TYPES Profile)
  endif()
else()
  set(allowableBuildTypes Debug MinSizeRel None RelWithDebInfo Release Coverage Profile)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "${allowableBuildTypes}")
  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
  elseif(NOT CMAKE_BUILD_TYPE IN_LIST allowableBuildTypes)
    message(FATAL_ERROR "Invalid build type : ${CMAKE_BUILD_TYPE}")
  endif()
endif()

if(POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif()

if(POLICY CMP0083)
  cmake_policy(SET CMP0083 NEW)
  include(CheckPIESupported)
  check_pie_supported(OUTPUT_VARIABLE output LANGUAGES CXX)
  if(NOT CMAKE_CXX_LINK_PIE_SUPPORTED)
    message(WARNING "PIE is not supported at link time: ${output}.\n" "PIE link options will not be passed to linker.")
  else()
    set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
    message(STATUS "PIE link options will be passed to linker.")
  endif()
else()
  set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
endif()

# Optional IPO. Do not use IPO if it's not supported by compiler.
#include(CheckIPOSupported)
#check_ipo_supported(RESULT aResult OUTPUT aOutput)
#if(aResult)
#  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
#else()
#  message(WARNING "IPO is not supported:# ${aOutput}")
#endif()

include(Sanitizers)

include(DependencyGraph)
gen_dep_graph(OUTPUT_DIR ${CMAKE_INSTALL_PREFIX} TARGET_NAME dependency-graph OUTPUT_TYPE pdf)

include(Tools)
clang_tidy(-format-style=file -checks=* -header-filter='${CMAKE_SOURCE_DIR}/*')
cppcheck(--enable=warning,performance,portability,missingInclude --template="[{severity}][{id}] {message} {callstack} \(On {file}:{line}\)" --suppress=missingIncludeSystem --quiet --verbose --force)
include_what_you_use(-Xiwyu)

#if(BUILD_ANALYSIS OR BUILD_OLD_WAVEDUMP)
#  find_package(ROOT 6.14 REQUIRED)
#  include(${ROOT_USE_FILE})
#endif()

add_subdirectory(libs)

if(ENABLE_EXTRAS)
  add_subdirectory(extras)
endif()

add_subdirectory(apps)

if(ENABLE_TESTS)
  add_subdirectory(tests)
endif()

if(ENABLE_DOCS)
  add_subdirectory(docs)
endif()
