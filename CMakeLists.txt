#########################################################
#               CMakeLists.txt to build DAQ             #
#                 author LAGARDE Francois               #
#########################################################
cmake_minimum_required(VERSION 3.9...3.16.4 FATAL_ERROR)

project(YAODAQ VERSION "0.0.1.0" DESCRIPTION "YAODAQ for RPClab" HOMEPAGE_URL "https://github.com/RPClab/YAODAQ" LANGUAGES CXX)

# Ask CMake to output a compile_commands.json file for use with things like Vim YCM.
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

# set the C++ standard to C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_SOURCE_DIR}/cmake" "${CMAKE_SOURCE_DIR}/extern")

# guard against in-source builds
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif()

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/bin" CACHE PATH "Default Install Prefix" FORCE)
  message(STATUS "Install Prefix set to : ${CMAKE_INSTALL_PREFIX}")
endif()

set(LIBRARY_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}/lib")
set(RUNTIME_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}")
set(ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}/lib")
set(INCLUDE_OUTPUT_DIRECTORY  "${CMAKE_INSTALL_PREFIX}/include")
set(CMAKE_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}/cmake")

set(CMAKE_MACOSX_RPATH TRUE)
set(CMAKE_SKIP_BUILD_RPATH  FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "${LIBRARY_OUTPUT_DIRECTORY}")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
link_directories("${CMAKE_INSTALL_PREFIX}/lib/")

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
include(CheckIPOSupported)
check_ipo_supported(RESULT result OUTPUT output)
if(result)
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
  message(WARNING "IPO is not supported: ${output}")
endif()

include(Architectures)

option(ENABLE_TESTS "Include tests folder" ON)
option(ENABLE_DOCS "Include docs folder" ON)
option(BUILD_ANALYSIS "Build the Analysis code" ON)
option(BUILD_WEBSOCKETSERVER "Build the WebSocket Server" ON)

option(ANALYSER "I'M AN ANALYSIST" OFF)
option(CONFIGURATOR "Compile Configurator" ON)
option(CAEN_HARDWARE "Compile CAEN Hardware library" ON)
option(ENABLE_DOCTESTS "Include tests in the library. Setting this to OFF will remove all doctest related code.
                        Tests in tests/*.cpp will still be enabled." OFF)








include(${CMAKE_SOURCE_DIR}/settings.cmake)

if(BUILD_ANALYSIS)
    find_package(ROOT 6 REQUIRED)
    include(${ROOT_USE_FILE})
endif()




find_package(ZLIB)

include(fmt)
include(CLI11)
include(toml11)
include(spdlog)

if(NOT ${ANALYSER}) 
  include(CAENlibs)
  include(IXWebSocket)
  include(elogpp)
  if(${CONFIGURATOR})
    include(soci)
  endif()
endif()





add_subdirectory(source)

add_subdirectory(apps)

if(ENABLE_TESTS)
  add_subdirectory(tests)
endif()

if(ENABLE_DOCS)
  add_subdirectory(docs)
endif()
