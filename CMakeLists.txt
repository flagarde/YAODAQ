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
                    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DCAENHVWrapper_demo=ON -DINSTALL_ALL=ON
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






list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})




#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED)

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})







## Include headers
include_directories(include)

link_directories(${CMAKE_INSTALL_PREFIX}/lib)

## Sources

add_library( libb STATIC src/flash.cpp src/WDconfig.cpp src/WDplot.cpp src/fft.cpp src/keyb.cpp src/X742CorrectionRoutines.cpp src/spi.cpp)
add_dependencies(libb CAENlibs)
add_executable(WaveDump src/WaveDump.cpp)
link_libraries(WaveDump libb ${ROOT_LIBRARIES} CAENlibs CAENDigitizer m)

target_link_libraries(WaveDump ${ROOT_LIBRARIES} CAENDigitizer libb m)

## Install executable
install(TARGETS WaveDump RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
