###################################
#   CMakeLists.txt to build DAQ   #
#   Author LAGARDE Francois       #
###################################
cmake_minimum_required(VERSION 3.9...3.16.4 FATAL_ERROR)

project(YAODAQ VERSION "0.0.1.0" DESCRIPTION "YAODAQ for RPClab" HOMEPAGE_URL "https://github.com/RPClab/YAODAQ" LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_SOURCE_DIR}/cmake" "${CMAKE_SOURCE_DIR}/cmake/scripts" "${CMAKE_SOURCE_DIR}/external")

include(Options.cmake)

include(GenerateReadMe)
generate_readme()

# Ask CMake to output a compile_commands.json file for use with things like Vim YCM.
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
set(CMAKE_LINK_DEPENDS_NO_SHARED TRUE)

include(Colors)

include(Architectures)

# set the C++ standard to C++ 17
include(CXXStandards)
cxx_17()

if(APPLE)
  set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl/" CACHE STRING "Set openssl root dir")
endif()

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

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/bin" CACHE PATH "Default Install Prefix" FORCE)
  message(STATUS "Install Prefix set to : ${CMAKE_INSTALL_PREFIX}")
endif()

set(LIBRARY_OUTPUT_DIR "${CMAKE_INSTALL_PREFIX}/lib")
set(RUNTIME_OUTPUT_DIR "${CMAKE_INSTALL_PREFIX}")
set(ARCHIVE_OUTPUT_DIR "${CMAKE_INSTALL_PREFIX}/lib")
set(INCLUDE_OUTPUT_DIR "${CMAKE_INSTALL_PREFIX}/include")
set(CMAKE_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}/cmake")

set(CMAKE_MACOSX_RPATH TRUE)
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "${LIBRARY_OUTPUT_DIR}")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
link_directories("${LIBRARY_OUTPUT_DIR}")

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
  else() #
    set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
    message(STATUS "PIE link options will be passed to linker.")
  endif()
else()
  set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
endif()

# Optional IPO. Do not use IPO if it's not supported by compiler.
include(CheckIPOSupported)
check_ipo_supported(RESULT aResult OUTPUT aOutput)
if(aResult)
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
  message(WARNING "IPO is not supported:# ${aOutput}")
endif()

include(Sanitizers)

include(DependencyGraph)
gen_dep_graph(OUTPUT_DIR ${CMAKE_INSTALL_PREFIX} TARGET_NAME dependency-graph OUTPUT_TYPE pdf)

include(Tools)
clang_tidy(-format-style=file -checks=* -header-filter='${CMAKE_SOURCE_DIR}/*')
cppcheck(--enable=warning,performance,portability,missingInclude --template="[{severity}][{id}] {message} {callstack} \(On {file}:{line}\)" --suppress=missingIncludeSystem --quiet --verbose --force)
include_what_you_use(-Xiwyu)

if(BUILD_WEBSERVER)
  include(jsroot)
  include(civet)
endif()

if(BUILD_ANALYSIS)
  find_package(ROOT 6.14 REQUIRED)
  include(${ROOT_USE_FILE})
endif()

add_subdirectory(libs)

if(ENABLE_EXTRAS)
  add_subdirectory(extras)
endif()

add_subdirectory(apps)

if(ENABLE_TESTS)
  include(CTest)
  add_subdirectory(tests)
endif()

if(ENABLE_DOCS)
  add_subdirectory(docs)
endif()
