include(ExternalProject)

if (NOT TARGET doctest)
# ----- doctest package -----
ExternalProject_Add(
    doctest
    GIT_REPOSITORY ${doctest_repository}
    GIT_TAG ${doctest_version}
    TIMEOUT 10
    UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
    PREFIX ${CMAKE_BINARY_DIR}/doctest
)
endif()
