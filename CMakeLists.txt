###################################
#   CMakeLists.txt to build DAQ   #
#   Author LAGARDE Francois       #
###################################
cmake_minimum_required(VERSION 3.10...3.17.2 FATAL_ERROR)

project(YAODAQ VERSION "0.0.1.0" DESCRIPTION "YAODAQ for RPClab" HOMEPAGE_URL "https://github.com/RPClab/YAODAQ" LANGUAGES CXX)

# Download CMakeCM
set(CMAKECM_BASE_URL "https://raw.githubusercontent.com/flagarde/CMakeCM/master" CACHE STRING "URL of the CMakeCM package")
set(CMAKECM_SHA256 "9493e474c71d279c2b263afffb3ced51cfaf937a7c7103de444ab306f5f23df3" CACHE STRING "SHA256 of CMakeCM")
set(CMAKECM_INSTALL_DIR "${CMAKE_BINARY_DIR}/cmake/CMakeCM" CACHE STRING "Path where to install CMakeCM")

##set(CMAKECM_MODULES_DIR "${CMAKECM_INSTALL_DIR}/Modules")
file(DOWNLOAD "${CMAKECM_BASE_URL}/CMakeCM.cmake" "${CMAKECM_INSTALL_DIR}/CMakeCM.cmake" INACTIVITY_TIMEOUT 5 TIMEOUT 1 EXPECTED_HASH SHA256=${CMAKECM_SHA256})
include("${CMAKECM_INSTALL_DIR}/CMakeCM.cmake")

#PreventInSourceBuilds is in  common but try to avoid downloading many files
include(PreventInSourceBuilds)
PreventInSourceBuilds()

include(Common)

default_install_prefix("${CMAKE_SOURCE_DIR}/bin")
cxx_17()

include(Formatters)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_CURRENT_SOURCE_DIR}/cmake" "${CMAKE_CURRENT_SOURCE_DIR}/cmake/scripts" "${CMAKE_CURRENT_SOURCE_DIR}/external" "${CMAKE_CURRENT_SOURCE_DIR}")

include(Settings)

include(doctest_project)

include(Messages)
include(Colors)

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

# FIXME
set(CMAKE_PREFIX_PATH "${CMAKE_OUTPUT_DIRECTORY}" "${CMAKE_INSTALL_PREFIX}/lib/cmake" "${CMAKE_INSTALL_PREFIX}/lib64/cmake")

# Install doctest to allow testing exectuables and test inside executables
include(CTest)

# If we want to have test in the executables
if(ENABLE_DOCTESTS)
  add_dependencies(YAODAQ::doctest)
  add_definitions(-DENABLE_DOCTEST_IN_LIBRARY)
endif()

include(ProcessorCount)
processorcount(N)
if(NOT N EQUAL 0)
  set(CTEST_BUILD_FLAGS -j${N})
endif()

if(WIN32)
  # FIXME
  set(CMAKE_SHARED_LINKER_FLAGS_COVERAGE "" CACHE STRING "Linker")
  set(CMAKE_EXE_LINKER_FLAGS_COVERAGE "" CACHE STRING "Linker")
  set(CMAKE_EXE_LINKER_FLAGS_PROFILE "" CACHE STRING "Linker")
  set(CMAKE_SHARED_LINKER_FLAGS_PROFILE "" CACHE STRING "Linker")
endif()

include(GenerateReadMe)
generate_readme()

include(Architectures)

set(CMAKE_MESSAGE_CONTEXT_SHOW TRUE)

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
