include_guard(GLOBAL)

include(CPM)
cpm()

if(NOT DEFINED NET-SNMP_REPOSITORY)
  set(NET-SNMP_REPOSITORY "https://github.com/net-snmp/net-snmp.git")
endif()

if(NOT DEFINED NET-SNMP_TAG)
  set(NET-SNMP_TAG "v5.9.1")
endif()

set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
include(OpenSSL)
include(Zlib-ng)

CPMAddPackage(NAME net-snmp
               GIT_REPOSITORY ${NET-SNMP_REPOSITORY}
               GIT_TAG ${NET-SNMP_TAG}
               FETCHCONTENT_UPDATES_DISCONNECTED ${IS_OFFLINE}
               OPTIONS "${net-snmp_OPTIONS}"
              )

if(net-snmp_ADDED)
  add_custom_target(compile-net-snmp ALL "make" "${net-snmp_BINARY_DIR}/Makefile" WORKING_DIRECTORY "${net-snmp_BINARY_DIR}")
  add_custom_target(install-net-snmp ALL "make" "install" WORKING_DIRECTORY "${net-snmp_BINARY_DIR}")
  if(NOT EXISTS ${net-snmp_BINARY_DIR}/Makefile)
    add_custom_target(configure-net-snmp ALL "sh" "${net-snmp_SOURCE_DIR}/configure" "CFLAGS=-fPIC" "CPPFLAGS=-fPIC" "--disable-agent" "--disable-applications" "--disable-manuals" "--disable-scripts" "--disable-mibs" "--prefix=${CMAKE_INSTALL_PREFIX}" "--disable-embedded-perl"
      "--without-rpm" "--with-sys-contact=\"\"" "--with-sys-location=\"\"" "--with-logfile=\"none\"" "--with-persistent-directory=\"\"" "--with-copy-persistent-files=\"no\"" "--disable-perl-cc-checks" "--with-defaults" "--enable-shared=\"off\""
      "--with-zlib=${ZLIB_ROOT}" "--enable-internal-md5" WORKING_DIRECTORY "${net-snmp_BINARY_DIR}")
      add_dependencies(compile-net-snmp configure-net-snmp)
  endif()
  add_library(net-snmp INTERFACE)
  message(WARN "${net-snmp_SOURCE_DIR}/include")
  target_link_libraries(net-snmp INTERFACE Threads::Threads INTERFACE netsnmp INTERFACE netsnmpagent INTERFACE netsnmphelpers INTERFACE netsnmpmibs INTERFACE OpenSSL::Crypto)
  target_include_directories(net-snmp INTERFACE ${CMAKE_INSTALL_FULL_INCLUDEDIR})
  add_dependencies(install-net-snmp compile-net-snmp)
  add_dependencies(net-snmp install-net-snmp)
  add_library(net-snmp::net-snmp ALIAS net-snmp)
endif()
