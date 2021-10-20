####################################
#                                  #
#  OPTIONS FOR EXTERNAL PACKAGES.  #
#                                  #
####################################

####################################
#        NECESSARY PACKAGES        #
####################################

#When on github action
if(DEFINED ENV{CI})
  set(CMMM_PROVIDER "github")
  set(PROVIDER_URL "https://github.com")
else()
  set(CMMM_PROVIDER "gitlab")
  set(PROVIDER_URL "https://gitlab.com")
endif()

#//////////////////////////////////#
#            CPM.CMake             #
#//////////////////////////////////#
set(CPM_DEFAULT_VERSION "0.34.0")
if(NOT DEFINED ENV{CI})
  set(CPM_URL "https://gitlab.com/external-packages/cpm.cmake/-/raw/v${CPM_DEFAULT_VERSION}/cmake/CPM.cmake")
endif()

#//////////////////////////////////#
#             OpenSSL              #
#//////////////////////////////////#
set(OPENSSLCMAKE_REPOSITORY "${PROVIDER_URL}/openssl-cmake.git")
set(OPENSSLCMAKE_TAG "v2.1")
set(USE_SYSTEM_OPENSSL OFF)
set(OPENSSL_VERSION "1.1.1l")
set(OPENSSL_URL "https://www.openssl.org/source/openssl-${OPENSSL_VERSION}.tar.gz")
set(OPENSSL_URL_HASH "SHA256=0b7a3e5e59c34827fe0c3a74b7ec8baef302b98fa80088d7f9153aa16fa76bd1")

# Zlib-ng
set(USE_SYSTEM_ZLIB OFF)
set(ZLIB-NG_REPOSITORY "https://gitlab.com/ExternalRepositories/zlib-ng.git")
set(ZLIB-NG_TAG "2.0.5")

# IXWebSocket
set(IXWEBSOCKET_REPOSITORY "https://gitlab.com/ExternalRepositories/IXWebSocket.git")
set(IXWEBSOCKET_TAG "v11.2.8")

# Fmt
set(FMT_REPOSITORY "https://gitlab.com/ExternalRepositories/fmt.git")
set(FMT_TAG "8.0.1")

# Spdlog
set(SPDLOG_REPOSITORY "https://gitlab.com/ExternalRepositories/spdlog.git")
set(SPDLOG_TAG "v1.9.1")

# magic_enum
set(MAGIC_ENUM_REPOSITORY "https://gitlab.com/ExternalRepositories/magic_enum.git")
set(MAGIC_ENUM_TAG "master")

# FlakedTuna
set(FLAKEDTUNA_REPOSITORY "https://gitlab.com/flagarde/FlakedTuna.git")
set(FLAKEDTUNA_TAG "1.1")

# Jsoncpp
set(JSONCPP_REPOSITORY "https://gitlab.com/ExternalRepositories/jsoncpp.git")
set(JSONCPP_TAG "master")

# Toml11
set(TOML11_REPOSITORY "https://gitlab.com/ExternalRepositories/toml11.git")
set(TOML11_TAG "master")

# SourceLocation
set(SOURCELOCATION_REPOSITORY "https://gitlab.com/flagarde/source_location.git")
set(SOURCELOCATION_TAG "main")

# SourceLocation
set(NLOHMANN_REPOSITORY "https://gitlab.com/external-packages/nlohmann_json_cmake_fetchcontent")
set(NLOHMANN_TAG "v3.10.4")


## FOR TESTS ##

# Doctest
set(DOCTEST_REPOSITORY "https://gitlab.com/ExternalRepositories/doctest.git")
set(DOCTEST_TAG "2.4.6")

## FOR APPS ##

# CLI11
set(CLI11_REPOSITORY "https://gitlab.com/ExternalRepositories/CLI11.git")
set(CLI11_TAG "v2.0.0")

####################################
#          EXTRA PACKAGES          #
####################################


####################################
#         GENERAL OPTIONS          #
####################################
option(BUILD_TESTS "Build the tests" TRUE)
option(BUILD_EXAMPLES "Build the examples" TRUE)
option(BUILD_DOCS "Build the docs" TRUE)
option(BUILD_EXTRAS "Build the extras" TRUE)




##





# elogpp
set(ELOGPP_REPOSITORY "https://gitlab.com/flagarde/elogpp")
set(ELOGPP_TAG "main")




set(serial_repository "https://gitlab.com/flagarde/serial")
set(serial_version "master")

# CAENlibs
set(CAENLIBS_REPOSITORY "https://gitlab.com/flagarde/CAENlibs.git")
set(CAENLIBS_TAG "main")


# soci
set(soci_repository "https://github.com/SOCI/soci.git")
set(soci_version "master")


# civet
set(civet_repository "https://github.com/civetweb/civetweb.git")
set(civet_version "master")

# jsroot
set(JSROOT_REPOSITORY "https://gitlab.com/ExternalRepositories/jsroot.git")
set(JSROOT_TAG "6.1.0")

# cereal
set(cereal_repository "https://github.com/USCiLab/cereal.git")
set(cereal_version "master")

set(SNMP_repository "https://github.com/RPClab/net-snmp.git")
set(SNMP_version "master")




# Compiling ROOT can be very long so :
option(USE_ROOT_SYSTEM "Try to find a ROOT installation and use it" OFF)
option(BUILD_ROOT "Build ROOT Folder" ON)
#set(LOOK_FOR_ROOT_VERSION "6.20")
# If it fails to find ROOT V${LOOK_FOR_ROOT_VERSION} or greater then Download and Install it !
#set(ROOT_repository "https://root.cern/download/root_v${LOOK_FOR_ROOT_VERSION}.source.tar.gz")
#

option(BUILD_ANALYSIS "Build the Analysis code" ON)
option(BUILD_WEBSOCKETSERVER "Build the WebSocket Server" ON)
option(BUILD_WEBSERVER "Build the civet webserver" ON)
option(BUILD_CONFIGURATOR "Build Configurator" ON)
option(BUILD_CONTROLLER "Build Controller" ON)
option(BUILD_LOGGER "Build Logger" ON)
option(BUILD_SNMP "Build SNMP" ON)
option(BUILD_WIENER "Build WIENER" ON)
option(BUILD_DAQ "Build DAQ" ON)
option(BUILD_Serial "Build Serial" OFF)

option(BUILD_LCIO "Build LCIO" OFF)
option(BUILD_MPOD "Build Mpod executasble" ON)
option(CAEN_HARDWARE "Compile CAEN Hardware library" ON)

option(ENABLE_ALL_WARNINGS "Compile with all warnings for the major compilers" ON)
option(ENABLE_EFFECTIVE_CXX "Enable Effective C++ warnings" OFF)
option(GENERATE_DEPENDENCY_DATA "Generates .d files with header dependencies" ON)

option(ENABLE_DOCTESTS "Include tests in the library. Setting this to OFF will remove all doctest related code.
                        Tests in tests/*.cpp will still be enabled." OFF
       )
