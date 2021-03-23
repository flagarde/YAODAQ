if(NOT TARGET elogpp)
  include(ExternalProject)
  # ----- elogpp_project package -----
  externalproject_add(
    elogpp_project
    DEPENDS jsoncpp_project
    DEPENDS OpenSSL::SSL
    GIT_REPOSITORY ${ELOGPP_REPOSITORY}
    GIT_TAG ${ELOGPP_TAG}
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD} -DCMAKE_CXX_STANDARD_REQUIRED=${CMAKE_CXX_STANDARD_REQUIRED} -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS}
               -DCMAKE_POSITION_INDEPENDENT_CODE=${CMAKE_POSITION_INDEPENDENT_CODE} -DOPENSSL_ROOT_DIR=${CMAKE_INSTALL_PREFIX} -DUSE_SYSTEM_JSONCPP=TRUE -Djsoncpp_DIR="${CMAKE_INSTALL_PREFIX}/lib/cmake/jsoncpp/"
    PREFIX ${CMAKE_BINARY_DIR}/elogpp_project
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    LOG_DOWNLOAD ON
    )
  add_library(elogpp_internal INTERFACE)
  add_dependencies(elogpp_internal elogpp_project)
  target_link_libraries(elogpp_internal INTERFACE elogpp INTERFACE OpenSSL::SSL)
  target_include_directories(elogpp_internal INTERFACE "${INCLUDE_OUTPUT_DIR}/elogpp")
  add_library(elogpp::elogpp ALIAS elogpp_internal)
endif()
