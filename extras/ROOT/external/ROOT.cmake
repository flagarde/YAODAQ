if(USE_ROOT)
 find_package(ROOT ${LOOK_FOR_ROOT_VERSION} QUIET)
endif()

if(NOT ROOT_FOUND OR NOT USE_ROOT) 
  include(FetchContent)
  message(AUTHOR_WARNING "ROOT not found. Compiling it")
  FetchContent_Declare(
   ROOT
   URL ${ROOT_repository}
  )
  FetchContent_GetProperties(ROOT)
  if(NOT root_POPULATED)
    FetchContent_Populate(ROOT)
    ##FIXME USE FETCH_CONTENT FOR ZLIB AND SSL IF ROOT HAVE TO BE BUILD
    execute_process(COMMAND cmake -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -Dbuiltin_fftw3=ON -Dbuiltin_afterimage=ON -Dbuiltin_ftgl=ON -Dbuiltin_freetype=ON -Dbuiltin_glew=ON -Dbuiltin_pcre=ON -Dbuiltin_zlib=ON -Dbuiltin_lzma=ON -Dbuiltin_davix=ON -Dbuiltin_gsl=ON -Dbuiltin_cfitsio=ON -Dbuiltin_xrootd=ON -Dbuiltin_tbb=ON  -Dssl=OFF -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD} -DCMAKE_CXX_STANDARD_REQUIRED=${CMAKE_CXX_STANDARD_REQUIRED} -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS} -DCMAKE_POSITION_INDEPENDENT_CODE=${CMAKE_POSITION_INDEPENDENT_CODE} -Droot7=OFF -Dmathmore=OFF -Dtmva=OFF -Dhdfs=OFF -Dpython=OFF -Druntime_cxxmodules=OFF -Dxrootd=OFF -Dxml=OFF -Dvdt=OFF -Dtable=OFF -Dshadowpw=OFF -Dsapdb=OFF -Drfio=OFF -Dqtgsi=OFF -Dqt=OFF -Dpythia8=OFF -Dpythia6=OFF -Dhttp=OFF -Dgfal=OFF -S "${CMAKE_BINARY_DIR}/_deps/root-src" -B "${CMAKE_BINARY_DIR}/_deps/root-build")
    execute_process(COMMAND cmake --build "${CMAKE_BINARY_DIR}/_deps/root-build" -j${N} )
    execute_process(COMMAND cmake --install "${CMAKE_BINARY_DIR}/_deps/root-build"  )
    message(INFO "Now it should be available !")
  endif()
endif()


find_package(ROOT "${LOOK_FOR_ROOT_VERSION}" REQUIRED)
include("${ROOT_USE_FILE}")
add_library(ROOT INTERFACE)
target_link_libraries(ROOT INTERFACE "${ROOT_LIBRARIES}")
#string(REPLACE " " ";" ROOT_CXX_FLAGS ${ROOT_CXX_FLAGS})
#target_compile_definitions(ROOT INTERFACE "${ROOT_CXX_FLAGS}")
target_include_directories(ROOT INTERFACE "${ROOT_INCLUDE_DIRS}")

