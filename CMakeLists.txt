########################################################
# Top-level CMakeLists.txt to build DAQ
# @author LAGARDE Francois
cmake_minimum_required(VERSION 3.1)
########################################################

project(DAQ VERSION 1.0 LANGUAGES C CXX)


OPTION(RASPBERRY "I'M RASPBERRY" OFF)
OPTION(ANALYSER "I'M AN ANALYSIST" OFF)

set(CMAKE_INSTALL_PREFIX "")
if ("${CMAKE_INSTALL_PREFIX}" STREQUAL "")
    set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/bin)
else()
endif()

# set the C++ standard to C++ 17
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/)
set(CMAKE_CXX_FLAGS  "-fPIC ${CMAKE_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS  "-fPIC ${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath=${CMAKE_INSTALL_PREFIX}/lib/" )


if(NOT ${ANALYSER}) 
set(CAENlibs_repository "https://github.com/RPClab/CAENlibs.git")
set(CAENlibs_version "master")

set(IXWebSocket_repository "https://github.com/RPClab/IXWebSocket.git")
set(IXWebSocket_version "master")

set(elogpp_repository "https://github.com/RPClab/elogpp.git")
set(elogpp_version "master")

set(zeromq_repository "https://github.com/RPClab/libzmq.git")
set(zeromq_version "master")

set(czmq_repository "https://github.com/RPClab/czmq.git")
set(czmq_version "master")



include(ExternalProject)
if(RASPBERRY) 
message(STATUS "You said I'm a RASPBERRY !!!")
endif()

# ----- zeromq package -----
#ExternalProject_Add( 
#                    zeromq
#                    GIT_REPOSITORY ${zeromq_repository}
#                    GIT_TAG ${zeromq_version}
#                    GIT_PROGRESS TRUE
#                    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DENABLE_ASAN=ON -DBUILD_TESTS=OFF
#                    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/zeromq-prefix
#                    SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/zeromq
#                    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
#                    )   

# ----- czmq package -----
#ExternalProject_Add( 
#                    czmq
#                    GIT_REPOSITORY ${czmq_repository}
#                    GIT_TAG ${czmq_version}
#                    GIT_PROGRESS TRUE
#                    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DLIBZMQ_LIBRARIES="${CMAKE_INSTALL_PREFIX}/lib/libzmq"
#                    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/czmq-prefix
#                    SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/czmq
#                    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
#                    ) 
#add_dependencies(czmq zeromq)





# ----- CAENlibs package -----
ExternalProject_Add( 
                    CAENlibs
                    GIT_REPOSITORY ${CAENlibs_repository}
                    GIT_TAG ${CAENlibs_version}
                    GIT_PROGRESS TRUE
                    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DINSTALL_ALL_DEMO=OFF -DRASPBERRY=${RASPBERRY} -DINSTALL_CAENVME=ON -DINSTALL_CAENComm=ON -DINSTALL_CAENHVWrapper=ON -DINSTALL_CAENDigitizer=ON
                    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/CAENlibs-prefix
                    SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/CAENlibs
                    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
                    )                       


# ----- WebSocket package -----
ExternalProject_Add(
                    WebSocket
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

if (UNIX)
   SET(CAEN_FLAGS "-DUNIX" "-DLINUX" CACHE INTERNAL " " FORCE )
endif (UNIX)

if (WIN32)
   SET(CAEN_FLAGS "-D_WIN32" "-DWIN32" CACHE INTERNAL " " FORCE )
endif (WIN32)
include_directories("${CMAKE_INSTALL_PREFIX}/include/CAEN")
include_directories("${CMAKE_INSTALL_PREFIX}/include/ixwebsocket")
add_definitions(${CAEN_FLAGS})
link_libraries(CAENVME CAENComm ${CMAKE_THREAD_LIBS_INIT})
else()
link_libraries(${CMAKE_THREAD_LIBS_INIT})
endif()

link_directories(${CMAKE_INSTALL_PREFIX}/lib)

include_directories("${CMAKE_INSTALL_PREFIX}/include")



find_package (Threads)








list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED COMPONENTS Spectrum)
include(${ROOT_USE_FILE})


#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})







## Include headers
include_directories(include)

link_directories(${CMAKE_INSTALL_PREFIX}/lib)

## Sources
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

ROOT_GENERATE_DICTIONARY(G__Channel Channel.hpp LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/include/ChannelLinkDef.hpp)

#---Create a shared library with geneated dictionary
add_library(Channel SHARED src/Channel.cpp G__Channel.cxx)
target_link_libraries(Channel ${ROOT_LIBRARIES})

ROOT_GENERATE_DICTIONARY(G__Event Event.hpp LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/include/EventLinkDef.hpp)

#---Create a shared library with geneated dictionary
add_library(Event SHARED src/Event.cpp G__Event.cxx)
target_link_libraries(Event ${ROOT_LIBRARIES})


if(NOT ${ANALYSER}) 
add_library( libb STATIC src/flash.cpp src/WDconfig.cpp src/FileManager.cpp src/X742CorrectionRoutines.cpp src/spi.cpp src/Plotter.cpp src/Digitizer.cpp src/WebServer.cpp)


add_executable(WaveDump src/WaveDump.cpp)

add_dependencies(Event CAENlibs elogpp )
find_package(ZLIB)
include_directories(${ZLIB_INCLUDE_DIRS})
 find_package(OpenSSL REQUIRED)
  add_definitions(${OPENSSL_DEFINITIONS})
  message(STATUS "OpenSSL: " ${OPENSSL_VERSION})
  include_directories(${OPENSSL_INCLUDE_DIR})
target_link_libraries(WaveDump ${ROOT_LIBRARIES} CAENDigitizer libb  elogcpp jsoncpp m crypto Channel Event  ${ZLIB_LIBRARIES} ${OPENSSL_LIBRARIES} ixwebsocket)


## Install executable
install(TARGETS WaveDump RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
endif()
##add_subdirectory(Analysis)

add_executable(Analysis Analysis/Analysis.cpp)
target_link_libraries(Analysis ${ROOT_LIBRARIES} Event Channel)
install(TARGETS Analysis RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})

