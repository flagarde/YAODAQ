if(NOT TARGET doctest)
include(ExternalProject)
# ----- doctest package -----
ExternalProject_Add(
    doctest
    GIT_REPOSITORY ${doctest_repository}
    GIT_TAG ${doctest_version}
    TIMEOUT 10
    UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    CONFIGURE_COMMAND ""
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD} -DCMAKE_CXX_STANDARD_REQUIRED=${CMAKE_CXX_STANDARD_REQUIRED} -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS} -DCMAKE_POSITION_INDEPENDENT_CODE=${CMAKE_POSITION_INDEPENDENT_CODE} -DDOCTEST_WITH_TESTS=OFF -DDOCTEST_NO_INSTALL=ON -DDOCTEST_WITH_MAIN_IN_STATIC_LIB=OFF -DCMAKE_INSTALL_INCLUDEDIR=${CMAKE_INSTALL_PREFIX}/include
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
    PREFIX ${CMAKE_BINARY_DIR}/doctest
)
endif()
