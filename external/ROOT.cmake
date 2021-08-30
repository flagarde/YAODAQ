include_guard(GLOBAL)

include(CPM)

if(NOT DEFINED USE_SYSTEM_ROOT)
  set(USE_SYSTEM_ROOT OFF)
endif()

if(NOT DEFINED ROOT_VERSION)
  set(ROOT_VERSION "6.24.02")
endif()

if(NOT DEFINED ROOT_URL)
  set(ROOT_URL "https://root.cern/download/root_v${ROOT_VERSION}.source.tar.gz")
endif()

if(NOT DEFINED ROOT_URL_HASH)
  set(ROOT_URL_HASH "SHA256=0507e1095e279ccc7240f651d25966024325179fa85a1259b694b56723ad7c1c")
endif()

message(WARN "1******************* ${ROOT_DIR}")

macro(compileROOT)
  message(WARN "ROOT version ${ROOT_VERSION} not found ! Compiling it !")
  include(ProcessorCount)
  ProcessorCount(NUMBER_CORES)
  if(NUMBER_CORES EQUAL 0)
    set(NUMBER_CORES 1)
  endif()
  #FIX THIS
  declare_option(REPOSITORY root OPTION alien VALUE OFF)
  declare_option(REPOSITORY root OPTION arrow VALUE OFF)
  declare_option(REPOSITORY root OPTION asimage VALUE OFF)
  declare_option(REPOSITORY root OPTION builtin_afterimage VALUE ON)
  declare_option(REPOSITORY root OPTION builtin_cfitsio VALUE ON)
  declare_option(REPOSITORY root OPTION builtin_clang VALUE ON)
  declare_option(REPOSITORY root OPTION builtin_davix VALUE ON)
  declare_option(REPOSITORY root OPTION builtin_fftw3 VALUE ON)
  declare_option(REPOSITORY root OPTION builtin_freetype VALUE ON)
  declare_option(REPOSITORY root OPTION builtin_ftgl VALUE ON)
  declare_option(REPOSITORY root OPTION builtin_gl2ps VALUE ON)
  declare_option(REPOSITORY root OPTION builtin_glew VALUE ON)
  declare_option(REPOSITORY root OPTION builtin_gsl VALUE ON)
  declare_option(REPOSITORY root OPTION builtin_llvm VALUE ON)
  declare_option(REPOSITORY root OPTION builtin_lz4 VALUE ON)
  declare_option(REPOSITORY root OPTION builtin_lzma VALUE ON)
  declare_option(REPOSITORY root OPTION builtin_openssl VALUE OFF)
  declare_option(REPOSITORY root OPTION builtin_pcre VALUE OFF)
  declare_option(REPOSITORY root OPTION builtin_tbb VALUE ON)
  declare_option(REPOSITORY root OPTION builtin_unuran VALUE ON)
  declare_option(REPOSITORY root OPTION builtin_vc VALUE ON)
  declare_option(REPOSITORY root OPTION builtin_vdt VALUE ON)
  declare_option(REPOSITORY root OPTION builtin_veccore VALUE ON)
  declare_option(REPOSITORY root OPTION builtin_xrootd VALUE ON)
  declare_option(REPOSITORY root OPTION builtin_xxhash VALUE ON)
  declare_option(REPOSITORY root OPTION builtin_zlib VALUE OFF)
  declare_option(REPOSITORY root OPTION builtin_zstd VALUE ON)
  declare_option(REPOSITORY root OPTION ccache VALUE OFF)
  declare_option(REPOSITORY root OPTION distcc VALUE OFF)
  declare_option(REPOSITORY root OPTION cefweb VALUE OFF)
  declare_option(REPOSITORY root OPTION clad VALUE OFF)
  declare_option(REPOSITORY root OPTION cocoa VALUE OFF)
  declare_option(REPOSITORY root OPTION coverage VALUE OFF)
  declare_option(REPOSITORY root OPTION cuda VALUE OFF)
  declare_option(REPOSITORY root OPTION cudnn VALUE OFF)
  declare_option(REPOSITORY root OPTION cxxmodules VALUE OFF)
  declare_option(REPOSITORY root OPTION dataframe VALUE OFF)
  declare_option(REPOSITORY root OPTION davix VALUE OFF)
  declare_option(REPOSITORY root OPTION dev VALUE OFF)
  declare_option(REPOSITORY root OPTION exceptions VALUE ON)
  declare_option(REPOSITORY root OPTION fftw3 VALUE ON)
  declare_option(REPOSITORY root OPTION fitsio VALUE OFF)
  declare_option(REPOSITORY root OPTION fortran VALUE OFF)
  declare_option(REPOSITORY root OPTION gdml VALUE OFF)
  declare_option(REPOSITORY root OPTION gfal VALUE OFF)
  declare_option(REPOSITORY root OPTION gviz VALUE OFF)
  declare_option(REPOSITORY root OPTION http VALUE OFF)
  declare_option(REPOSITORY root OPTION fcgi VALUE OFF)
  declare_option(REPOSITORY root OPTION imt VALUE OFF)
  declare_option(REPOSITORY root OPTION mathmore VALUE OFF)
  declare_option(REPOSITORY root OPTION memstat VALUE OFF)
  declare_option(REPOSITORY root OPTION mlp VALUE OFF)
  declare_option(REPOSITORY root OPTION minuit2 VALUE OFF)
  declare_option(REPOSITORY root OPTION monalisa VALUE OFF)
  declare_option(REPOSITORY root OPTION mpi VALUE OFF)
  declare_option(REPOSITORY root OPTION mysql VALUE OFF)
  declare_option(REPOSITORY root OPTION odbc VALUE OFF)
  declare_option(REPOSITORY root OPTION opengl VALUE OFF)
  declare_option(REPOSITORY root OPTION oracle VALUE OFF)
  declare_option(REPOSITORY root OPTION pgsql VALUE OFF)
  declare_option(REPOSITORY root OPTION pyroot VALUE OFF)
  declare_option(REPOSITORY root OPTION pythia6 VALUE OFF)
  declare_option(REPOSITORY root OPTION pythia8 VALUE OFF)
  declare_option(REPOSITORY root OPTION roofit VALUE OFF)
  declare_option(REPOSITORY root OPTION webgui VALUE OFF)
  declare_option(REPOSITORY root OPTION root7 VALUE OFF)
  declare_option(REPOSITORY root OPTION runtime_cxxmodules VALUE OFF)
  declare_option(REPOSITORY root OPTION sqlite VALUE OFF)
  declare_option(REPOSITORY root OPTION ssl VALUE OFF)
  declare_option(REPOSITORY root OPTION tmva VALUE OFF)
  declare_option(REPOSITORY root OPTION tmva-cpu VALUE OFF)
  declare_option(REPOSITORY root OPTION tmva-pymva VALUE OFF)
  declare_option(REPOSITORY root OPTION vdt VALUE OFF)
  declare_option(REPOSITORY root OPTION x11 VALUE OFF)
  declare_option(REPOSITORY root OPTION xml VALUE OFF)
  declare_option(REPOSITORY root OPTION xrootd VALUE OFF)
  print_options(REPOSITORY  root)

  include(OpenSSL)
  include(Zlib-ng)
  include(FetchContent)
  FetchContent_Declare(ROOT URL ${ROOT_URL} URL_HASH ${ROOT_URL_HASH})
  FetchContent_GetProperties(ROOT)
  if(NOT ${root_POPULATED} STREQUAL "True")
    FetchContent_Populate(ROOT)
    execute_process(COMMAND ${CMAKE_COMMAND}
                    -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}/root_${ROOT_VERSION}
                    -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
                    -DCXX_STANDARD_REQUIRED=ON
                    -DCXX_EXTENSIONS=OFF
                    -Dalien=OFF
                    -Dbuiltin_pcre=OFF
                    -Dbuiltin_openssl=OFF
                    -Dpythia6=OFF
                    -Dpythia8=OFF
                    -Dfortran=OFF
                    -Doracle=OFF
                    -Dcoverage=OFF
                    -Dcuda=OFF
                    -Dccache=OFF
                    -Ddistcc=OFF
                    -Dbuiltin_zlib=OFF
                    -Dcudnn=OFF
                    -Ddcache=OFF
                    -Djemalloc=OFF
                    -Dmemstat=OFF
                    -Dpyroot_legacy=OFF
                    -Dtcmalloc=OFF
                    -Dtesting=OFF
                    -Drootbench=OFF
                    -Droottest=OFF
                    -Dxproofd=OFF
                    -Ddev=OFF
                    -DCMAKE_POSITION_INDEPENDENT_CODE=${CMAKE_POSITION_INDEPENDENT_CODE}
                    -S "${CMAKE_BINARY_DIR}/_deps/root-src"
                    -B "${CMAKE_BINARY_DIR}/_deps/root-build")
  endif()
  execute_process(COMMAND ${CMAKE_COMMAND} --build "${CMAKE_BINARY_DIR}/_deps/root-build" -j${NUMBER_CORES} COMMAND_ECHO STDOUT)
  execute_process(COMMAND ${CMAKE_COMMAND} --install "${CMAKE_BINARY_DIR}/_deps/root-build" COMMAND_ECHO STDOUT)
endmacro()

if(USE_SYSTEM_ROOT)
  find_package(ROOT ${ROOT_VERSION} CONFIG)
  if(NOT ROOT_FOUND)
    compileROOT()
  else()
    set(COMPILE_ROOT FALSE)
    include("${ROOT_USE_FILE}")
  endif()
else()
  message(WARN "2******************* ${ROOT_DIR}")
  compileROOT()
endif()

message(WARN "3******************* ${ROOT_DIR}")
set(ROOT_DIR "${CMAKE_INSTALL_PREFIX}/root_${ROOT_VERSION}/cmake/")
message(WARN "4******************* ${ROOT_DIR}")
find_package(ROOT REQUIRED)
include("${ROOT_USE_FILE}")