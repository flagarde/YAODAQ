if(NOT TARGET spdlog::spdlog)
  include(ExternalProject)
  include(fmt)
  # misc tweakme options
  if(NOT WIN32)
    set(SPDLOG_WCHAR_SUPPORT OFF CACHE BOOL "Support wchar api" FORCE)
    set(SPDLOG_WCHAR_FILENAMES OFF CACHE BOOL "Support wchar filenames" FORCE)
  endif()
  if(NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(SPDLOG_CLOCK_COARSE OFF CACHE BOOL "Use the much faster (but much less accurate) CLOCK_REALTIME_COARSE instead of the regular clock" FORCE)
  endif()
  # ----- spdlog_project package -----
  externalproject_add(
    spdlog_project
    DEPENDS fmt_project
    GIT_REPOSITORY ${SPDLOG_REPOSITORY}
    GIT_TAG ${SPDLOG_VERSION}
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
    CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
               -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
               -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
               -DCMAKE_CXX_STANDARD_REQUIRED=${CMAKE_CXX_STANDARD_REQUIRED}
               -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS}
               -DCMAKE_POSITION_INDEPENDENT_CODE=${CMAKE_POSITION_INDEPENDENT_CODE}
               -SPDLOG_MASTER_PROJECT=OFF
               -DSPDLOG_BUILD_ALL=OFF
               -DSPDLOG_ENABLE_PCH=ON
               -DSPDLOG_BUILD_EXAMPLE=OFF
               -DSPDLOG_BUILD_EXAMPLE_HO=OFF
               -DSPDLOG_BUILD_TESTS=OFF
               -DSPDLOG_BUILD_TESTS_HO=OFF
               -DSPDLOG_BUILD_BENCH=OFF
               -DSPDLOG_SANITIZE_ADDRESS=OFF
               -DSPDLOG_BUILD_WARNINGS=OFF
               -DSPDLOG_INSTALL=ON
               -DSPDLOG_FMT_EXTERNAL=ON
               -DSPDLOG_FMT_EXTERNAL_HO=OFF
               -DSPDLOG_NO_EXCEPTIONS=OFF
               -DSPDLOG_WCHAR_SUPPORT=${SPDLOG_WCHAR_SUPPORT}
               -DSPDLOG_WCHAR_FILENAMES=${SPDLOG_WCHAR_FILENAMES}
               -DSPDLOG_CLOCK_COARSE=${SPDLOG_CLOCK_COARSE}
               -DSPDLOG_PREVENT_CHILD_FD=OFF
               -DSPDLOG_NO_THREAD_ID=OFF
               -DSPDLOG_NO_TLS=OFF
               -DSPDLOG_NO_ATOMIC_LEVELS=OFF
               -Dfmt_DIR=${CMAKE_OUTPUT_DIRECTORY}
    PREFIX ${CMAKE_BINARY_DIR}/spdlog_project
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    LOG_DOWNLOAD ON
    )
  add_library(spdlog_internal INTERFACE)
  add_dependencies(spdlog_internal spdlog_project fmt::fmt)
  target_compile_definitions(spdlog_internal INTERFACE SPDLOG_FMT_EXTERNAL)
  target_link_libraries(spdlog_internal INTERFACE "spdlog$<$<CONFIG:Debug>:d>" INTERFACE fmt::fmt)
  target_include_directories(spdlog_internal INTERFACE "${INCLUDE_OUTPUT_DIR}/spdlog")
  add_library(spdlog::spdlog ALIAS spdlog_internal)
endif()
