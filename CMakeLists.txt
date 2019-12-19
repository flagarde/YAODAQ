#########################################################
#               CMakeLists.txt to build DAQ             #
#                 author LAGARDE Francois               #
#########################################################

cmake_minimum_required(VERSION 3.9...3.16 FATAL_ERROR)

project(DAQ VERSION 1.0.0.0 DESCRIPTION "DAQ for RPClab" HOMEPAGE_URL "https://github.com/RPClab/DAQ" LANGUAGES C CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/)

include(ExternalProject)
include(ConfigSafeGuards)
include(CodeCoverage)
include(Doctest)

find_package(Git REQUIRED)

option(ANALYSER "I'M AN ANALYSIST" OFF)
option(CAEN_HARDWARE "Compile CAEN Hardware library" ON)
option(ENABLE_DOCTESTS "Include tests in the library. Setting this to OFF will remove all doctest related code.
                        Tests in tests/*.cpp will still be enabled." OFF)
                        
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/bin" CACHE PATH "Default Install Prefix" FORCE)
  message(STATUS "Install Prefix set to : ${CMAKE_INSTALL_PREFIX}")
endif()

# Ask CMake to output a compile_commands.json file for use with things like Vim YCM.
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

cmake_policy(SET CMP0048 NEW)

cmake_policy(SET CMP0083 NEW)
include(CheckPIESupported)
check_pie_supported(OUTPUT_VARIABLE output LANGUAGES CXX)
if(NOT CMAKE_CXX_LINK_PIE_SUPPORTED)
  message(WARNING "PIE is not supported at link time: ${output}.\n"
                  "PIE link options will not be passed to linker.")
else()
  set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
  message(STATUS "PIE link options will be passed to linker.")
endif()

include(CheckIPOSupported)
# Optional IPO. Do not use IPO if it's not supported by compiler.
check_ipo_supported(RESULT result OUTPUT output)
if(result)
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
  message(WARNING "IPO is not supported: ${output}")
endif()


set(LIBRARY_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}/lib")
set(RUNTIME_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}")
set(ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}/lib")
set(INCLUDE_OUTPUT_DIRECTORY  "${CMAKE_INSTALL_PREFIX}/include")
set(CMAKE_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}/cmake")

# set the C++ standard to C++ 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)

set(CMAKE_MACOSX_RPATH TRUE)
set(CMAKE_SKIP_BUILD_RPATH  TRUE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "${LIBRARY_OUTPUT_DIRECTORY}")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

include(Architectures)

if(NOT ${ANALYSER}) 
set(CAENlibs_repository "https://github.com/RPClab/CAENlibs.git")
set(CAENlibs_version "master")

set(IXWebSocket_repository "https://github.com/RPClab/IXWebSocket.git")
set(IXWebSocket_version "master")

set(elogpp_repository "https://github.com/RPClab/elogpp.git")
set(elogpp_version "master")

find_package (Threads)


include(FetchContent)


# ----- CAENlibs package -----
ExternalProject_Add( 
                    CAENlibs
                   GIT_REPOSITORY ${CAENlibs_repository}
                    GIT_TAG ${CAENlibs_version}
                    GIT_PROGRESS TRUE
                    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DCAENALL=ON
                    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/CAENlibs-prefix
                    SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/CAENlibs
                    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
                    )                       
if(UNIX)
   set(CAEN_DEFS "-DUNIX" "-DLINUX" CACHE INTERNAL " " FORCE)
elseif(WIN32)
   set(CAEN_DEFS "-D_WIN32" "-DWIN32" CACHE INTERNAL " " FORCE)
endif()

mark_as_advanced(FORCE CAEN_DEFS)

# ----- WebSocket package -----
ExternalProject_Add(
                    WebSocket DEPENDS 
                    GIT_REPOSITORY ${IXWebSocket_repository}
                    GIT_TAG ${IXWebSocket_version}
                    GIT_PROGRESS TRUE
                    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DUSE_TLS=1 -DUSE_WS=0
                    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/IXWebSocket-prefix
                    SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/IXWebSocket
                    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
                    )

# ----- elogpp package -----
ExternalProject_Add(
                    elogpp
                    GIT_REPOSITORY ${elogpp_repository}
                    GIT_TAG ${elogpp_version}
                    GIT_PROGRESS TRUE
                    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
                    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/elogpp-prefix
                    SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/elogpp
                    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
                    )
else()
link_libraries(${CMAKE_THREAD_LIBS_INIT})
endif()

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})

add_subdirectory(source)
add_subdirectory(apps)

add_subdirectory(tests)
