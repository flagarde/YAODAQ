# - Find Net-SNMP
#
# -*- cmake -*-
#
# Find the Net-SNMP module
#
#  NETSNMP_INCLUDE_DIR - where to find Net-SNMP.h, etc.
#  NETSNMP_LIBRARIES   - List of libraries when using Net-SNMP.
#  NETSNMP_FOUND       - True if Net-SNMP found.

if(NETSNMP_INCLUDE_DIR)
  # Already in cache, be silent
  set(NETSNMP_FIND_QUIETLY TRUE)
endif(NETSNMP_INCLUDE_DIR)

find_path(NETSNMP_INCLUDE_DIR snmp.h ${CMAKE_INSTALL_PREFIX}/include/net-snmp/library)

set(NETSNMP_NAMES netsnmp)
find_library(NETSNMP_LIBRARY NAMES ${NETSNMP_NAMES} PATHS ${CMAKE_INSTALL_PREFIX}/lib ${CMAKE_INSTALL_PREFIX}/lib32 ${CMAKE_INSTALL_PREFIX}/lib64)

set(NETSNMPAGENT_NAMES netsnmpagent)
find_library(NETSNMPAGENT_LIBRARY NAMES ${NETSNMPAGENT_NAMES} PATHS ${CMAKE_INSTALL_PREFIX}/lib ${CMAKE_INSTALL_PREFIX}/lib32 ${CMAKE_INSTALL_PREFIX}/lib64)

set(NETSNMPHELPERS_NAMES netsnmphelpers)
find_library(NETSNMPHELPERS_LIBRARY NAMES ${NETSNMPHELPERS_NAMES} PATHS ${CMAKE_INSTALL_PREFIX}/lib ${CMAKE_INSTALL_PREFIX}/lib32 ${CMAKE_INSTALL_PREFIX}/lib64)

set(NETSNMPMIBS_NAMES netsnmpmibs)
find_library(NETSNMPMIBS_LIBRARY NAMES ${NETSNMPMIBS_NAMES} PATHS ${CMAKE_INSTALL_PREFIX}/lib ${CMAKE_INSTALL_PREFIX}/lib32 ${CMAKE_INSTALL_PREFIX}/lib64)

set(NETSNMPTRAPD_NAMES netsnmptrapd)
find_library(NETSNMPTRAPD_LIBRARY NAMES ${NETSNMPTRAPD_NAMES} PATHS ${CMAKE_INSTALL_PREFIX}/lib ${CMAKE_INSTALL_PREFIX}/lib32 ${CMAKE_INSTALL_PREFIX}/lib64)

set(NETSNMP_LIBRARIES ${NETSNMP_LIBRARY} ${NETSNMPAGENT_LIBRARY} ${NETSNMPHELPERS_LIBRARY} ${NETSNMPMIBS_LIBRARY} # ${NETSNMPTRAPD_LIBRARY}
    )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(NETSNMP DEFAULT_MSG NETSNMP_INCLUDE_DIR NETSNMP_LIBRARIES)

mark_as_advanced(NETSNMP_LIBRARY NETSNMPAGENT_LIBRARY NETSNMPHELPERS_LIBRARY NETSNMPMIBS_LIBRARY NETSNMPTRAPD_LIBRARY NETSNMP_INCLUDE_DIR)

if(NOT TARGET net-snmp AND NOT NETSNMP_FOUND)
  include(ExternalProject)
  include(openssl)
  # ----- net-snmp package -----
  externalproject_add(
    net-snmp
    DEPENDS openssl_project
    DEPENDS zlib-ng_project
    GIT_REPOSITORY ${SNMP_repository}
    GIT_TAG ${SNMP_version}
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
    CONFIGURE_COMMAND ""
    BUILD_COMMAND
      "sh" "${CMAKE_BINARY_DIR}/net-snmp/configure" "CFLAGS=-fPIC" "CPPFLAGS=-fPIC" "--disable-agent" "--disable-applications" "--disable-manuals" "--disable-scripts" "--disable-mibs" "--prefix=${CMAKE_INSTALL_PREFIX}" "--disable-embedded-perl"
      "--without-rpm" "--with-sys-contact=\"\"" "--with-sys-location=\"\"" "--with-logfile=\"none\"" "--with-persistent-directory=\"\"" "--with-copy-persistent-files=\"no\"" "--disable-perl-cc-checks" "--with-defaults" "--enable-shared=\"off\""
      "--with-zlib=${ZLIB_ROOT}" "--enable-internal-md5"
    PREFIX ${CMAKE_BINARY_DIR}/net-snmp-prefix
    SOURCE_DIR ${CMAKE_BINARY_DIR}/net-snmp
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    LOG_DOWNLOAD ON
    UPDATE_DISCONNECTED TRUE
    )
endif()
set(NETSNMP_FOUND TRUE CACHE BOOL "" FORCE)
find_package(Threads REQUIRED)
add_library(SNMP INTERFACE)
add_dependencies(SNMP net-snmp)
target_link_libraries(
  SNMP
  INTERFACE netsnmp
  INTERFACE netsnmpagent
  INTERFACE netsnmphelpers
  INTERFACE netsnmpmibs
  INTERFACE Threads::Threads
  INTERFACE OpenSSL::Crypto
  )
target_include_directories(SNMP INTERFACE "${CMAKE_INSTALL_PREFIX}/include/net-snmp")
