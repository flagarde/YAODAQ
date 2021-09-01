include_guard(GLOBAL)

include(CPM)
cpm()

if(NOT DEFINED USE_SYSTEM_ROOT)
  set(USE_SYSTEM_ROOT OFF)
endif()

if(NOT DEFINED ROOT_VERSION)
  set(ROOT_VERSION "6.24.02")
endif()

if(USE_SYSTEM_ROOT)
  find_package(ROOT ${ROOT_VERSION} CONFIG)
  if(NOT ROOT_FOUND)
    set(COMPILE_ROOT TRUE)
  else()
    set(COMPILE_ROOT FALSE)
    include("${ROOT_USE_FILE}")
  endif()
else()
  set(COMPILE_ROOT TRUE)
endif()

if(COMPILE_ROOT)

  if(NOT DEFINED ROOT_REPOSITORY)
    set(ROOT_REPOSITORY "https://github.com/flagarde/root.git")
  endif()

  if(NOT DEFINED ROOT_TAG)
    set(ROOT_TAG "FetchContent")
  endif()

  #BUILTIN
  #ROOT can't finf libz-ng as it clear variable
  include(OpenSSL)
  include(Zlib-ng)
  declare_option(REPOSITORY ROOT OPTION ROOT_BUILTINS VALUE "ZLIB;OpenSSL")
  declare_option(REPOSITORY ROOT OPTION builtin_afterimage VALUE ON)
  declare_option(REPOSITORY ROOT OPTION builtin_cfitsio VALUE ON)
  declare_option(REPOSITORY ROOT OPTION builtin_clang VALUE ON)
  declare_option(REPOSITORY ROOT OPTION builtin_cling VALUE ON)
  declare_option(REPOSITORY ROOT OPTION builtin_davix VALUE ON)
  declare_option(REPOSITORY ROOT OPTION builtin_fftw3 VALUE ON)
  declare_option(REPOSITORY ROOT OPTION builtin_freetype VALUE ON)
  declare_option(REPOSITORY ROOT OPTION builtin_ftgl VALUE ON)
  declare_option(REPOSITORY ROOT OPTION builtin_gl2ps VALUE ON)
  declare_option(REPOSITORY ROOT OPTION builtin_glew VALUE ON)
  declare_option(REPOSITORY ROOT OPTION builtin_gsl VALUE ON)
  declare_option(REPOSITORY ROOT OPTION builtin_llvm VALUE ON)
  declare_option(REPOSITORY ROOT OPTION builtin_lz4 VALUE ON)
  declare_option(REPOSITORY ROOT OPTION builtin_lzma VALUE OFF)
  declare_option(REPOSITORY ROOT OPTION builtin_nlohmannjson VALUE ON)
  declare_option(REPOSITORY ROOT OPTION builtin_openssl VALUE OFF)
  declare_option(REPOSITORY ROOT OPTION builtin_openui5 VALUE ON)
  declare_option(REPOSITORY ROOT OPTION builtin_pcre VALUE ON)
  declare_option(REPOSITORY ROOT OPTION builtin_tbb VALUE ON)
  declare_option(REPOSITORY ROOT OPTION builtin_unuran VALUE ON)
  declare_option(REPOSITORY ROOT OPTION builtin_vc VALUE ON)
  declare_option(REPOSITORY ROOT OPTION builtin_vdt VALUE ON)
  declare_option(REPOSITORY ROOT OPTION builtin_veccore VALUE ON)
  declare_option(REPOSITORY ROOT OPTION builtin_xrootd VALUE ON)
  declare_option(REPOSITORY ROOT OPTION builtin_xxhash VALUE ON)
  declare_option(REPOSITORY ROOT OPTION builtin_zlib VALUE OFF)

  declare_option(REPOSITORY ROOT OPTION builtin_zstd VALUE ON)
  declare_option(REPOSITORY ROOT OPTION alien VALUE OFF)
  declare_option(REPOSITORY ROOT OPTION CMAKE_POSITION_INDEPENDENT_CODE VALUE ON)
  print_options(REPOSITORY  ROOT)
  CPMAddPackage(NAME ROOT
               GIT_REPOSITORY ${ROOT_REPOSITORY}
               GIT_TAG ${ROOT_TAG}
               FETCHCONTENT_UPDATES_DISCONNECTED ${IS_OFFLINE}
               OPTIONS ${ROOT_OPTIONS})
endif()
