########################################################
# Top-level CMakeLists.txt to build DAQ
# @author LAGARDE Francois
########################################################
cmake_minimum_required(VERSION 3.1)

project(DAQ VERSION 1.0 LANGUAGES C CXX)

option(ANALYSER "I'M AN ANALYSIST" OFF)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/bin" CACHE PATH "Default Install Prefix" FORCE)
  message(STATUS "Install Prefix set to : ${CMAKE_INSTALL_PREFIX}")
endif()

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

set(LIBRARY_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}/lib")
set(RUNTIME_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}/bin")
set(ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}/lib")
set(INCLUDE_OUTPUT_DIRECTORY  "${CMAKE_INSTALL_PREFIX}/include")
set(CMAKE_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}/cmake")

# set the C++ standard to C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/)
set(CMAKE_MACOSX_RPATH TRUE)
set(CMAKE_SKIP_BUILD_RPATH  TRUE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "${LIBRARY_OUTPUT_DIRECTORY}")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


if(NOT ${ANALYSER}) 
set(CAENlibs_repository "https://github.com/RPClab/CAENlibs2.git")
set(CAENlibs_version "master")

set(IXWebSocket_repository "https://github.com/RPClab/IXWebSocket.git")
set(IXWebSocket_version "master")

set(elogpp_repository "https://github.com/RPClab/elogpp.git")
set(elogpp_version "master")

find_package (Threads)

include(ExternalProject)
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

ROOT_GENERATE_DICTIONARY(G__Channel ${CMAKE_CURRENT_SOURCE_DIR}/include/Channel.hpp LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/include/ChannelLinkDef.hpp)
add_library(Channel OBJECT src/Channel.cpp G__Channel.cxx)
target_link_libraries(Channel INTERFACE ${ROOT_LIBRARIES})
target_include_directories(Channel PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
add_library(Channel_shared SHARED $<TARGET_OBJECTS:Channel>)
add_library(Channel_static STATIC $<TARGET_OBJECTS:Channel>)

ROOT_GENERATE_DICTIONARY(G__Event ${CMAKE_CURRENT_SOURCE_DIR}/include/Event.hpp LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/include/EventLinkDef.hpp)
add_library(Event OBJECT src/Event.cpp G__Event.cxx)
target_include_directories(Event PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_link_libraries(Event INTERFACE ${ROOT_LIBRARIES})
add_library(Event_shared SHARED $<TARGET_OBJECTS:Event>)
add_library(Event_static STATIC $<TARGET_OBJECTS:Event>)

add_executable(Analysis Analysis/Analysis.cpp)
target_link_libraries(Analysis ${ROOT_LIBRARIES} Event_static Channel_static)
target_include_directories(Analysis PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
install(TARGETS Analysis RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})

if(NOT ${ANALYSER}) 
    
    add_library(lib1 STATIC src/flash.cpp src/WDconfig.cpp src/X742CorrectionRoutines.cpp src/spi.cpp src/Digitizer.cpp)
    add_dependencies(lib1 CAENlibs)
    target_include_directories(lib1 PUBLIC "${INCLUDE_OUTPUT_DIRECTORY}/CAEN" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
    
    add_library(lib2 STATIC src/FileManager.cpp )
    add_dependencies(lib2 CAENlibs)
    target_include_directories(lib2 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/" PUBLIC "${INCLUDE_OUTPUT_DIRECTORY}/CAEN")
    
    add_library(web STATIC src/WebServer.cpp src/Plotter.cpp )
    add_dependencies(web elogpp WebSocket CAENlibs)
    target_include_directories(web PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/" PUBLIC "${INCLUDE_OUTPUT_DIRECTORY}/ixwebsocket" "${INCLUDE_OUTPUT_DIRECTORY}/CAEN")

    
    add_executable(WaveDump src/WaveDump.cpp)
    find_package(ZLIB)
    target_include_directories(WaveDump PRIVATE ${ZLIB_INCLUDE_DIRS})
    find_package(OpenSSL REQUIRED)
    add_dependencies(WaveDump lib1 lib2 web elogpp WebSocket CAENlibs)
    target_compile_definitions(WaveDump PRIVATE  ${OPENSSL_DEFINITIONS})
    message(STATUS "OpenSSL: " ${OPENSSL_VERSION})
    target_include_directories(WaveDump PRIVATE  ${OPENSSL_INCLUDE_DIR})
    target_include_directories(WaveDump PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/" PUBLIC "${INCLUDE_OUTPUT_DIRECTORY}/ixwebsocket" "${INCLUDE_OUTPUT_DIRECTORY}/CAEN" "${INCLUDE_OUTPUT_DIRECTORY}/elogpp")
    target_link_directories(WaveDump PRIVATE ${LIBRARY_OUTPUT_DIRECTORY})
    target_link_libraries(WaveDump ${ROOT_LIBRARIES} CAENDigitizer CAENComm lib1 lib2 web  elogcpp jsoncpp m crypto Channel_static Event_static  ${ZLIB_LIBRARIES} ${OPENSSL_LIBRARIES} ixwebsocket)
    install(TARGETS WaveDump RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
endif()



