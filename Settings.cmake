######################################
######################################
##                                  ##
##         GENERAL OPTIONS.         ##
##                                  ##
######################################
######################################
set(CI TRUE)
if(DEFINED ENV{CI} OR DEFINED CI)
  option(USE_SYSTEM_ROOT "Try to find a ROOT installation and use it" ON)
  option(BUILD_DOCS "Build the docs" FALSE)
  option(BUILD_TESTS "Build the tests" FALSE)
  option(BUILD_EXAMPLES "Build the examples" FALSE)
else()
  option(USE_SYSTEM_ROOT "Try to find a ROOT installation and use it" ON)
  option(BUILD_DOCS "Build the docs" TRUE)
  option(BUILD_TESTS "Build the tests" TRUE)
  option(BUILD_EXAMPLES "Build the examples" TRUE)
endif()

option(BUILD_EXTRAS "Build the extras" TRUE)
# Compiling ROOT can be very long so :

option(BUILD_ROOT "Build ROOT Folder" ON)

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

######################################
######################################
##                                  ##
##  OPTIONS FOR EXTERNAL PACKAGES.  ##
##                                  ##
######################################
######################################

# When on github action CI is defined
if(DEFINED ENV{CI})
  set(CMMM_PROVIDER "github")
  set(PROVIDER_URL "https://github.com")
else()
  set(CMMM_PROVIDER "gitlab")
  set(PROVIDER_URL "https://gitlab.com")
endif()

######################################
#              TESTING               #
######################################

#////////////////////////////////////#
#              Doctest               #
#////////////////////////////////////#
set(DOCTEST_TAG "2.4.6")
if(NOT DEFINED ENV{CI})
  set(DOCTEST_REPOSITORY "${PROVIDER_URL}/external-packages/doctest.git")
else()
  set(DOCTEST_REPOSITORY "${PROVIDER_URL}/onqtam/doctest.git")
endif()

######################################
#               Apps                 #
######################################

#////////////////////////////////////#
#               CLI11                #
#////////////////////////////////////#
set(CLI11_TAG "v2.1.2")
if(NOT DEFINED ENV{CI})
  set(CLI11_REPOSITORY "${PROVIDER_URL}/external-packages/cli11.git")
else()
  set(CLI11_REPOSITORY "${PROVIDER_URL}/CLIUtils/CLI11.git")
endif()

######################################
#         NECESSARY PACKAGES         #
######################################

#////////////////////////////////////#
#             CPM.CMake              #
#////////////////////////////////////#
set(CPM_DEFAULT_VERSION "0.34.0")
if(NOT DEFINED ENV{CI})
  set(CPM_URL "https://gitlab.com/external-packages/cpm.cmake/-/raw/v${CPM_DEFAULT_VERSION}/cmake/CPM.cmake")
else()
  set(CPM_URL "https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DEFAULT_VERSION}/CPM.cmake")
endif()

#////////////////////////////////////#
#              OpenSSL               #
#////////////////////////////////////#
set(USE_SYSTEM_OPENSSL OFF)
set(OPENSSLCMAKE_TAG "v2.1")
set(OPENSSLCMAKE_REPOSITORY "${PROVIDER_URL}/flagarde/openssl-cmake.git")
set(OPENSSL_VERSION "1.1.1l")
set(OPENSSL_URL "https://www.openssl.org/source/openssl-${OPENSSL_VERSION}.tar.gz")
set(OPENSSL_URL_HASH "SHA256=0b7a3e5e59c34827fe0c3a74b7ec8baef302b98fa80088d7f9153aa16fa76bd1")

#////////////////////////////////////#
#              Zlib-ng               #
#////////////////////////////////////#
set(USE_SYSTEM_ZLIB OFF)
set(ZLIB-NG_TAG "2.0.5")
if(NOT DEFINED ENV{CI})
  set(ZLIB-NG_REPOSITORY "${PROVIDER_URL}/external-packages/zlib-ng.git")
else()
  set(ZLIB-NG_REPOSITORY "${PROVIDER_URL}/zlib-ng/zlib-ng.git")
endif()

#////////////////////////////////////#
#            IXWebSocket             #
#////////////////////////////////////#
set(IXWEBSOCKET_TAG "master")
if(NOT DEFINED ENV{CI})
  set(IXWEBSOCKET_REPOSITORY "${PROVIDER_URL}/external-packages/IXWebSocket.git")
else()
  set(IXWEBSOCKET_REPOSITORY "${PROVIDER_URL}/machinezone/IXWebSocket.git")
endif()

#////////////////////////////////////#
#            FlakedTuna              #
#////////////////////////////////////#
set(FLAKEDTUNA_TAG "1.1")
set(FLAKEDTUNA_REPOSITORY "${PROVIDER_URL}/flagarde/FlakedTuna.git")

#////////////////////////////////////#
#                Fmt                 #
#////////////////////////////////////#
set(FMT_TAG "8.0.1")
if(NOT DEFINED ENV{CI})
  set(FMT_REPOSITORY "${PROVIDER_URL}/external-packages/fmt.git")
else()
  set(FMT_REPOSITORY "${PROVIDER_URL}/fmtlib/fmt.git")
endif()

#////////////////////////////////////#
#               Spdlog               #
#////////////////////////////////////#
set(SPDLOG_TAG "v1.9.1")
if(NOT DEFINED ENV{CI})
  set(SPDLOG_REPOSITORY "${PROVIDER_URL}/external-packages/spdlog.git")
else()
  set(SPDLOG_REPOSITORY "${PROVIDER_URL}/gabime/spdlog.git")
endif()

#////////////////////////////////////#
#             magic_enum             #
#////////////////////////////////////#
set(MAGIC_ENUM_TAG "v0.7.3")
if(NOT DEFINED ENV{CI})
  set(MAGIC_ENUM_REPOSITORY "${PROVIDER_URL}/external-packages/magic_enum.git")
else()
  set(MAGIC_ENUM_REPOSITORY "${PROVIDER_URL}/Neargye/magic_enum.git")
endif()

#////////////////////////////////////#
#              Jsoncpp               #
# TO REMOVE TO REMOVE TO REMOVE TO RE#
#////////////////////////////////////#
set(JSONCPP_TAG "master")
if(NOT DEFINED ENV{CI})
  set(JSONCPP_REPOSITORY "${PROVIDER_URL}/external-packages/jsoncpp.git")
else()
  set(JSONCPP_REPOSITORY "${PROVIDER_URL}/open-source-parsers/jsoncpp.git")
endif()

#////////////////////////////////////#
#              nlohmann              #
#////////////////////////////////////#
set(NLOHMANN_TAG "v3.10.4")
if(NOT DEFINED ENV{CI})
  set(NLOHMANN_REPOSITORY "${PROVIDER_URL}/external-packages/nlohmann_json_cmake_fetchcontent.git")
else()
  set(NLOHMANN_REPOSITORY "${PROVIDER_URL}/ArthurSonzogni/nlohmann_json_cmake_fetchcontent.git")
endif()

#////////////////////////////////////#
#            json-rpc-cxx            #
#////////////////////////////////////#
set(JSON_RPC_CXX_TAG "v0.3.0")
if(NOT DEFINED ENV{CI})
  set(JSON_RPC_CXX_REPOSITORY "${PROVIDER_URL}/external-packages/json-rpc-cxx.git")
else()
  set(JSON_RPC_CXX_REPOSITORY "${PROVIDER_URL}/jsonrpcx/json-rpc-cxx.git")
endif()

#////////////////////////////////////#
#              Toml11                #
#////////////////////////////////////#
set(TOML11_TAG "master")
if(NOT DEFINED ENV{CI})
  set(TOML11_REPOSITORY "${PROVIDER_URL}/external-packages/toml11.git")
else()
  set(TOML11_REPOSITORY "${PROVIDER_URL}/ToruNiina/toml11.git")
endif()

#////////////////////////////////////#
#            dotenv-cpp              #
#////////////////////////////////////#
set(DOTENV_TAG "master")
set(DOTENV_REPOSITORY "${PROVIDER_URL}/flagarde/dotenv-cpp")

#////////////////////////////////////#
#          source_location           #
#////////////////////////////////////#
set(SOURCELOCATION_TAG "main")
set(SOURCELOCATION_REPOSITORY "${PROVIDER_URL}/flagarde/source_location.git")

######################################
#           EXTRA PACKAGES           #
######################################

#////////////////////////////////////#
#              elogpp                #
#////////////////////////////////////#
set(ELOGPP_TAG "main")
set(ELOGPP_REPOSITORY "${PROVIDER_URL}/flagarde/elogpp.git")

#////////////////////////////////////#
#             CAENlibs               #
#////////////////////////////////////#
set(CAENLIBS_REPOSITORY "${PROVIDER_URL}/flagarde/CAENlibs.git")
set(CAENLIBS_TAG "main")

#////////////////////////////////////#
#               soci                 #
#////////////////////////////////////#
set(SOCI_TAG "master")
if(NOT DEFINED ENV{CI})
  set(SOCI_REPOSITORY "${PROVIDER_URL}/external-packages/soci.git")
else()
  set(SOCI_REPOSITORY "${PROVIDER_URL}/SOCI/soci.git")
endif()

#////////////////////////////////////#
#             net-snmp               #
#////////////////////////////////////#
set(NET-SNMP_TAG "v5.9.1")
if(NOT DEFINED ENV{CI})
  set(NET-SNMP_REPOSITORY "${PROVIDER_URL}/external-packages/net-snmp.git")
else()
  set(NET-SNMP_REPOSITORY "${PROVIDER_URL}/net-snmp/net-snmp.git")
endif()

#////////////////////////////////////#
#              jsroot                #
#////////////////////////////////////#
set(JSROOT_TAG "6.2.1")
if(NOT DEFINED ENV{CI})
  set(JSROOT_REPOSITORY "${PROVIDER_URL}/external-packages/jsroot.git")
else()
  set(JSROOT_REPOSITORY "${PROVIDER_URL}/root-project/jsroot.git")
endif()

#////////////////////////////////////#
#           cpp-linenoise            #
#////////////////////////////////////#
set(CPP_LINENOISE_TAG "main")
set(CPP_LINENOISE_REPOSITORY "${PROVIDER_URL}/flagarde/cpp-linenoise.git")



set(serial_repository "https://gitlab.com/flagarde/serial")
set(serial_version "master")
