#########################################################
#               CMakeLists.txt to build DAQ             #
#                 author LAGARDE Francois               #
#########################################################

cmake_minimum_required(VERSION 3.9...3.16 FATAL_ERROR)

project(DAQ VERSION "1.0.0.0" DESCRIPTION "DAQ for RPClab" HOMEPAGE_URL "https://github.com/RPClab/DAQ" LANGUAGES C CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/ ${CMAKE_SOURCE_DIR}/extern/)
set(GENERATED_HEADER_PATH ${CMAKE_BINARY_DIR}/generated/)
include_directories(${GENERATED_HEADER_PATH})



include(ConfigSafeGuards)
include(Documentation)

find_package(Git REQUIRED)

find_package(ROOT 6 REQUIRED)
include(${ROOT_USE_FILE})

option(ANALYSER "I'M AN ANALYSIST" OFF)
option(CAEN_HARDWARE "Compile CAEN Hardware library" ON)
option(ENABLE_DOCTESTS "Include tests in the library. Setting this to OFF will remove all doctest related code.
                        Tests in tests/*.cpp will still be enabled." OFF)
option(ENABLE_TEST "Include tests folder" ON)
                        
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/bin" CACHE PATH "Default Install Prefix" FORCE)
  message(STATUS "Install Prefix set to : ${CMAKE_INSTALL_PREFIX}")
endif()

# Ask CMake to output a compile_commands.json file for use with things like Vim YCM.
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

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

include(CheckIPOSupported)
# Optional IPO. Do not use IPO if it's not supported by compiler.
check_ipo_supported(RESULT result OUTPUT output)
if(result)
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
  message(WARNING "IPO is not supported: ${output}")
endif()


include(${CMAKE_SOURCE_DIR}/settings.cmake)

set(LIBRARY_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}/lib")
set(RUNTIME_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}")
set(ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}/lib")
set(INCLUDE_OUTPUT_DIRECTORY  "${CMAKE_INSTALL_PREFIX}/include")
set(CMAKE_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}/cmake")

# set the C++ standard to C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)

set(CMAKE_MACOSX_RPATH TRUE)
set(CMAKE_SKIP_BUILD_RPATH  FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "${LIBRARY_OUTPUT_DIRECTORY}")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
link_directories("${CMAKE_INSTALL_PREFIX}/lib/")

include(Architectures)


find_package (Threads)

if(NOT ${ANALYSER}) 
  include(CAENlibs)
  include(IXWebSocket)
  include(elogpp)
endif()

add_subdirectory(source)
add_subdirectory(apps)

if(ENABLE_TEST)
    include(Doctest)
    add_subdirectory(tests)
endif()
