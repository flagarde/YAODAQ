include(ExternalProject)

if (NOT TARGET jsoncpp)
# ----- jsoncpp package -----
ExternalProject_Add(
                    jsoncpp
                    GIT_REPOSITORY ${jsoncpp_repository}
                    GIT_TAG ${jsoncpp_version}
                    GIT_PROGRESS TRUE
                    GIT_SHALLOW TRUE
                    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD} -DCMAKE_CXX_STANDARD_REQUIRED=${CMAKE_CXX_STANDARD_REQUIRED} -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS} -DJSONCPP_WITH_TESTS=FALSE -DJSONCPP_WITH_POST_BUILD_UNITTEST=FALSE -DJSONCPP_WITH_PKGCONFIG_SUPPORT=FALSE -DJSONCPP_WITH_EXAMPLE=FALSE -DCMAKE_POSITION_INDEPENDENT_CODE=${CMAKE_POSITION_INDEPENDENT_CODE}
                    PREFIX ${CMAKE_BINARY_DIR}/jsoncpp-prefix
                    SOURCE_DIR ${CMAKE_BINARY_DIR}/jsoncpp
                    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
                    )
endif()
