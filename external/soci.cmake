include(ExternalProject)

if (NOT TARGET soci)
# ----- soci package -----
ExternalProject_Add(
                    soci
                    GIT_REPOSITORY ${soci_repository}
                    GIT_TAG ${soci_version}
                    GIT_PROGRESS TRUE
                    GIT_SHALLOW TRUE
                    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD} -DCMAKE_CXX_STANDARD_REQUIRED=${CMAKE_CXX_STANDARD_REQUIRED} -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS} -DSOCI_SHARED=OFF -DSOCI_STATIC=ON -DSOCI_TESTS=OFF -DSOCI_HAVE_BOOST=FALSE -DSOCI_HAVE_BOOST_DATE_TIME=FALSE -DLIB_SUFFIX="" -DWITH_BOOST=OFF -DSOCI_LIBDIR=lib -DCMAKE_POSITION_INDEPENDENT_CODE=${CMAKE_POSITION_INDEPENDENT_CODE}
                    GIT_SUBMODULES "" 
                    PREFIX ${CMAKE_BINARY_DIR}/soci-prefix
                    SOURCE_DIR ${CMAKE_BINARY_DIR}/soci
                    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
                    )
add_library(Soci INTERFACE)
target_link_libraries(Soci INTERFACE soci_core INTERFACE soci_empty )
target_include_directories(Soci INTERFACE ${DATABASES_INCLUDE_DIR} )
find_package(DB2)
if(${DB2_FOUND})
  target_link_libraries(Soci INTERFACE ${DB2_LIBRARIES} INTERFACE soci_db2)
  target_include_directories(Soci INTERFACE ${DB2_INCLUDE_DIR})
endif()
find_package(Firebird)
if(${FIREBIRD_FOUND})
  target_link_libraries(Soci INTERFACE ${FIREBIRD_LIBRARIES} INTERFACE soci_firebird)
  target_include_directories(Soci INTERFACE ${DATABASES_INCLUDE_DIR} )
endif()
find_package(MySQL)
if(${MYSQL_FOUND})
  target_link_libraries(Soci INTERFACE ${MYSQL_LIBRARIES} INTERFACE soci_mysql)
  target_include_directories(Soci INTERFACE ${MYSQL_INCLUDE_DIR})
endif()
if(${MYSQL_EMBEDDED_FOUND})
  target_link_libraries(Soci INTERFACE ${MYSQL_EMBEDDED_LIBRARIES} INTERFACE soci_mysql)
  target_include_directories(Soci INTERFACE ${MYSQL_EMBEDDED_LIB_DIR})
endif()
find_package(ODBC)
if(${ODBC_FOUND})
  target_link_libraries(Soci INTERFACE ${ODBC_LIBRARIES} INTERFACE soci_odbc)
  target_include_directories(Soci INTERFACE ${ODBC_INCLUDE_DIRS})
endif()
find_package(Oracle)
if(${ORACLE_FOUND})
  target_link_libraries(Soci INTERFACE ${ORACLE_LIBRARIES} INTERFACE soci_oracle)
  target_include_directories(Soci INTERFACE ${ORACLE_INCLUDE_DIR})
endif()
find_package(PostgreSQL)
if(${POSTGRESQL_FOUND})
  target_link_libraries(Soci INTERFACE ${POSTGRESQL_LIBRARIES} INTERFACE soci_postgresql)
  target_include_directories(Soci INTERFACE  ${POSTGRESQL_INCLUDE_DIRS})
endif()
find_package(SQLite3)
if(${SQLITE3_FOUND})
  target_link_libraries(Soci INTERFACE ${SQLITE3_LIBRARIES} INTERFACE soci_sqlite3)
  target_include_directories(Soci INTERFACE ${SQLITE3_INCLUDE_DIR})
endif()
endif()
