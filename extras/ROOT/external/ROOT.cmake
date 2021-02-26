include(PositionIndependentExecutable)

set(FETCHCONTENT_QUIET OFF)

if(NOT DEFINED ROOT_VERSION)
	set(ROOT_VERSION "6.22.06")
endif()

if(NOT DEFINED ROOT_INSTALL_PREFIX)
  set(ROOT_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/ROOT-v${ROOT_VERSION}")
endif()

if( USE_ROOT_SYSTEM )
  find_package(ROOT ${ROOT_VERSION} QUIET CONFIG)
endif()

if(NOT ROOT_FOUND)
  set(ROOT_DIR "${ROOT_INSTALL_PREFIX}")
  find_package(ROOT ${ROOT_VERSION} QUIET CONFIG)
endif()

if(NOT DEFINED ROOT_CMAKE_CXX_STANDARD)
  set(ROOT_CMAKE_CXX_STANDARD "17")
endif()

if(NOT DEFINED ROOT_CMAKE_CXX_STANDARD_REQUIRED)
  set(ROOT_CMAKE_CXX_STANDARD_REQUIRED "ON")
endif()

if(NOT DEFINED ROOT_CMAKE_CXX_EXTENSIONS)
  set(ROOT_CMAKE_CXX_EXTENSIONS "OFF")
endif()

if(NOT DEFINED ROOT_REPOSITORY)
  set(ROOT_REPOSITORY "https://root.cern/download/root_v${ROOT_VERSION}.source.tar.gz")
endif()

macro(compileROOT)
  include(FetchContent)
  FetchContent_Declare(ROOT URL ${ROOT_REPOSITORY} )
  FetchContent_GetProperties(ROOT)
  if(NOT ${root_POPULATED} STREQUAL "True")
    FetchContent_Populate(ROOT)
    execute_process(COMMAND ${CMAKE_COMMAND}
                    -DCMAKE_INSTALL_PREFIX=${ROOT_INSTALL_PREFIX}
                    -DCMAKE_CXX_STANDARD=${ROOT_CMAKE_CXX_STANDARD}
                    -DCMAKE_CXX_STANDARD_REQUIRED=${ROOT_CMAKE_CXX_STANDARD_REQUIRED}
                    -DCMAKE_CXX_EXTENSIONS=${ROOT_CMAKE_CXX_EXTENSIONS}
                    -DCMAKE_POSITION_INDEPENDENT_CODE=${CMAKE_POSITION_INDEPENDENT_CODE}
                    -Dalien=OFF
                    -Darrow=OFF
                    -Dasimage=OFF
                    -Dbuiltin_afterimage=OFF
                    -Dbuiltin_cfitsio=OFF
                    -Dbuiltin_clang=ON
                    -Dbuiltin_davix=OFF
                    -Dbuiltin_fftw3=OFF
                    -Dbuiltin_freetype=OFF
                    -Dbuiltin_ftgl=OFF
                    -Dbuiltin_gl2ps=OFF
                    -Dbuiltin_glew=OFF
                    -Dbuiltin_gsl=OFF
                    -Dbuiltin_llvm=ON
                    -Dbuiltin_lz4=OFF
                    -Dbuiltin_lzma=OFF
                    -Dbuiltin_openssl=OFF
                    -Dbuiltin_pcre=OFF
                    -Dbuiltin_tbb=OFF
                    -Dbuiltin_unuran=OFF
                    -Dbuiltin_vc=OFF
                    -Dbuiltin_vdt=OFF
                    -Dbuiltin_veccore=OFF
                    -Dbuiltin_xrootd=OFF
                    -Dbuiltin_xxhash=OFF
                    -Dbuiltin_zlib=ON
                    -Dbuiltin_zstd=ON
                    -Dccache=OFF
                    -Ddistcc=OFF
                    -Dcefweb=OFF
                    -Dclad=OFF
                    -Dcocoa=OFF
                    -Dcoverage=OFF
                    -Dcuda=OFF
                    -Dcudnn=OFF
                    -Dcxxmodules=OFF
                    -Ddataframe=OFF
                    -Ddavix=OFF
                    -Ddcache=OFF
                    -Ddev=OFF
                    -Dexceptions=ON
                    -Dfftw3=OFF
                    -Dfitsio=OFF
                    -Dfortran=OFF
                    -Dgdml=OFF
                    -Dgfal=OFF
                    -Dgviz=OFF
                    -Dhttp=OFF
                    -Dfcgi=OFF
                    -Dimt=OFF
                    -Djemalloc=OFF
                    -Dmathmore=OFF
                    -Dmemstat=OFF
                    -Dmlp=OFF
                    -Dminuit2=OFF
                    -Dmonalisa=OFF
                    -Dmpi=OFF
                    -Dmysql=OFF
                    -Dodbc=OFF
                    -Dopengl=OFF
                    -Doracle=OFF
                    -Dpgsql=OFF
                    -Dpyroot=OFF
                    -Dpythia6=OFF
                    -Dpythia8=OFF
                    -Droofit=OFF
                    -Dwebgui=OFF
                    -Droot7=OFF
                    -Druntime_cxxmodules=OFF
                    -Dsqlite=OFF
                    -Dssl=OFF
                    -Dtmva=OFF
                    -Dtmva-cpu=OFF
                    -Dtmva-pymva=OFF
                    -Dvdt=OFF
                    -Dx11=OFF
                    -Dxml=OFF
                    -Dxrootd=OFF
                    -S "${CMAKE_BINARY_DIR}/_deps/root-src" -B "${CMAKE_BINARY_DIR}/_deps/root-build")
    execute_process(COMMAND ${CMAKE_COMMAND} --build "${CMAKE_BINARY_DIR}/_deps/root-build" -j10)
    execute_process(COMMAND ${CMAKE_COMMAND} --install "${CMAKE_BINARY_DIR}/_deps/root-build")
  endif()
endmacro()

if(NOT ROOT_FOUND OR NOT USE_ROOT_SYSTEM)
  message(WARN "ROOT version ${ROOT_VERSION} not found ! Compiling it !")
  compileROOT()
endif()

find_package(ROOT ${ROOT_VERSION} REQUIRED CONFIG)
include("${ROOT_USE_FILE}")
add_library(ROOT INTERFACE)
target_link_libraries(ROOT INTERFACE "${ROOT_LIBRARIES}")
# string(REPLACE " " ";" ROOT_CXX_FLAGS ${ROOT_CXX_FLAGS}) target_compile_definitions(ROOT INTERFACE "${ROOT_CXX_FLAGS}")
target_include_directories(ROOT INTERFACE "${ROOT_INCLUDE_DIRS}")
