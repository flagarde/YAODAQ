########################################################
# Top-level CMakeLists.txt to build DAQ
# @author LAGARDE Francois
cmake_minimum_required(VERSION 3.1)
########################################################

project(DAQ VERSION 1.0 LANGUAGES C CXX)

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




set(CAENlibs_repository "https://github.com/RPClab/CAENlibs.git")
set(CAENlibs_version "master")

include(ExternalProject)

# ----- CAENlibs package -----
ExternalProject_Add( 
                    CAENlibs
                    GIT_REPOSITORY ${CAENlibs_repository}
                    GIT_TAG ${CAENlibs_version}
                    GIT_PROGRESS TRUE
                    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DCAENHVWrapper_demo=ON
                    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/CAENlibs-prefix
                    SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/CAENlibs
                    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
                    )                       



if (UNIX)
   SET(CAEN_FLAGS "-DUNIX" "-DLINUX" CACHE INTERNAL " " FORCE )
endif (UNIX)

if (WIN32)
   SET(CAEN_FLAGS "-D_WIN32" "-DWIN32" CACHE INTERNAL " " FORCE )
endif (WIN32)

link_directories(${CMAKE_INSTALL_PREFIX}/lib)
add_definitions(${CAEN_FLAGS})
include_directories("${CMAKE_INSTALL_PREFIX}/include")
include_directories("${CMAKE_INSTALL_PREFIX}/include/CAEN")

link_libraries(CAENVME CAENComm pthread)


















## Include headers
include_directories(include)

link_directories(${CMAKE_INSTALL_PREFIX}/lib)

## Sources

add_library( libb STATIC src/flash.c src/WDconfig.c src/WDplot.c src/fft.c src/keyb.c src/X742CorrectionRoutines.c src/spi.c)
add_dependencies(libb CAENlibs)
add_executable(WaveDump src/WaveDump.c)
link_libraries(WaveDump libb CAENlibs CAENDigitizer m)

target_link_libraries(WaveDump CAENDigitizer libb m)

## Install executable
install(TARGETS WaveDump RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})



#find_package(ROOT)
#if(ROOT_FOUND)
#include_directories(${ROOT_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
# add_executable(IvsU IvsU.cpp)
# if(CAEN_DRIVER)
#   add_dependencies(IvsU drivers)
#   endif()
# include_directories(${MariaDBClient_INCLUDE_DIRS})
#  target_link_libraries(IvsU ${MariaDBClient_LIBRARIES} pthread ${ROOT_LIBRARIES} ${Boost_LIBRARIES} Sauron )
#  install(TARGETS IvsU RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})


