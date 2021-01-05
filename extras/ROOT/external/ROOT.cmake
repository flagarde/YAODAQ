if(USE_ROOT)
  find_package(ROOT ${LOOK_FOR_ROOT_VERSION} QUIET)
endif()

if(NOT ROOT_FOUND OR NOT USE_ROOT)
  include(FetchContent)
  message(AUTHOR_WARNING "ROOT not found. Compiling it")
  fetchcontent_declare(ROOT URL ${ROOT_repository})
  fetchcontent_getproperties(ROOT)
  if(NOT root_POPULATED)
    fetchcontent_populate(ROOT)
    # FIXME USE FETCH_CONTENT FOR ZLIB AND SSL IF ROOT HAVE TO BE BUILD
    execute_process(
      COMMAND
        cmake -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -Dalien=OFF -Darrow=OFF -Dasimage=OFF -Dbuiltin_afterimage=OFF -Dbuiltin_cfitsio=OFF -Dbuiltin_clang=ON -Dbuiltin_davix=OFF -Dbuiltin_fftw3=OFF -Dbuiltin_freetype=OFF -Dbuiltin_ftgl=OFF -Dbuiltin_gl2ps=OFF -Dbuiltin_glew=OFF -Dbuiltin_gsl=OFF -Dbuiltin_llvm=ON -Dbuiltin_lz4=OFF -Dbuiltin_lzma=OFF -Dbuiltin_openssl=OFF -Dbuiltin_pcre=OFF -Dbuiltin_tbb=OFF -Dbuiltin_unuran=OFF -Dbuiltin_vc=OFF -Dbuiltin_vdt=OFF -Dbuiltin_veccore=OFF -Dbuiltin_xrootd=OFF -Dbuiltin_xxhash=OFF -Dbuiltin_zlib=ON -Dbuiltin_zstd=ON -Dccache=OFF -Ddistcc=OFF -Dcefweb=OFF -Dclad=OFF -Dcocoa=OFF -Dcoverage=OFF -Dcuda=OFF -Dcudnn=OFF -Dcxxmodules=OFF -Ddataframe=OFF -Ddavix=OFF -Ddcache=OFF -Ddev=OFF -Dexceptions=ON -Dfftw3=OFF -Dfitsio=OFF -Dfortran=OFF -Dgdml=OFF -Dgfal=OFF -Dgviz=OFF -Dhttp=OFF -Dfcgi=OFF -Dimt=OFF -Djemalloc=OFF -Dmathmore=OFF -Dmemstat=OFF -Dmlp=OFF -Dminuit2=OFF -Dmonalisa=OFF -Dmpi=OFF -Dmysql=OFF -Dodbc=OFF -Dopengl=OFF -Doracle=OFF -Dpgsql=OFF -Dpyroot=OFF -Dpythia6=OFF -Dpythia8=OFF -Droofit=OFF -Dwebgui=OFF -Droot7=OFF -Druntime_cxxmodules=OFF -Dsqlite=OFF -Dssl=OFF -Dtmva=OFF -Dtmva-cpu=OFF -Dtmva-pymva=OFF -Dvdt=OFF -Dx11=OFF -Dxml=OFF -Dxrootd=OFF -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD} -DCMAKE_CXX_STANDARD_REQUIRED=${CMAKE_CXX_STANDARD_REQUIRED} -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS}
        -DCMAKE_POSITION_INDEPENDENT_CODE=${CMAKE_POSITION_INDEPENDENT_CODE} -S "${CMAKE_BINARY_DIR}/_deps/root-src" -B "${CMAKE_BINARY_DIR}/_deps/root-build"
      )
    execute_process(COMMAND cmake --build "${CMAKE_BINARY_DIR}/_deps/root-build" -j${N})
    execute_process(COMMAND cmake --install "${CMAKE_BINARY_DIR}/_deps/root-build")
    message(INFO "Now it should be available !")
  endif()
endif()

find_package(ROOT "${LOOK_FOR_ROOT_VERSION}" REQUIRED)
include("${ROOT_USE_FILE}")
add_library(ROOT INTERFACE)
target_link_libraries(ROOT INTERFACE "${ROOT_LIBRARIES}")
# string(REPLACE " " ";" ROOT_CXX_FLAGS ${ROOT_CXX_FLAGS}) target_compile_definitions(ROOT INTERFACE "${ROOT_CXX_FLAGS}")
target_include_directories(ROOT INTERFACE "${ROOT_INCLUDE_DIRS}")
