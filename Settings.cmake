######################################
######################################
##                                  ##
##         GENERAL OPTIONS.         ##
##                                  ##
######################################
######################################

option(USE_SYSTEM_ROOT "Try to find a ROOT installation and use it" ON)
option(BUILD_DOCS "Build the docs" TRUE)
option(BUILD_TESTS "Build the tests" TRUE)
option(BUILD_EXAMPLES "Build the examples" TRUE)
option(BUILD_EXTRAS "Build the extras" TRUE)
# Compiling ROOT can be very long so :

option(BUILD_ROOT "Build ROOT Folder" OFF)

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

######################################
#              TESTING               #
######################################

#////////////////////////////////////#
#              Doctest               #
#////////////////////////////////////#
set(DOCTEST_TAG "v2.4.12")
set(DOCTEST_REPOSITORY "https://github.com/onqtam/doctest.git")

######################################
#               Apps                 #
######################################

#////////////////////////////////////#
#               CLI11                #
#////////////////////////////////////#
set(CLI11_TAG "v2.6.1")
set(CLI11_REPOSITORY "https://github.com/CLIUtils/CLI11.git")

######################################
#         NECESSARY PACKAGES         #
######################################

#////////////////////////////////////#
#             CPM.CMake              #
#////////////////////////////////////#
set(CPM_DEFAULT_VERSION "0.42.0")
set(CPM_URL "https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DEFAULT_VERSION}/CPM.cmake")

#////////////////////////////////////#
#              OpenSSL               #
#////////////////////////////////////#
set(USE_SYSTEM_OPENSSL OFF)
set(OPENSSLCMAKE_TAG "v2.4")
set(OPENSSLCMAKE_REPOSITORY "https://github.com/flagarde/openssl-cmake.git")
set(OPENSSL_VERSION "1.1.1l")
set(OPENSSL_URL "https://www.openssl.org/source/openssl-${OPENSSL_VERSION}.tar.gz")
set(OPENSSL_URL_HASH "SHA256=0b7a3e5e59c34827fe0c3a74b7ec8baef302b98fa80088d7f9153aa16fa76bd1")

#////////////////////////////////////#
#              Zlib-ng               #
#////////////////////////////////////#
set(USE_SYSTEM_ZLIB OFF)
set(ZLIB-NG_TAG "2.3.2")
set(ZLIB-NG_REPOSITORY "https://github.com/zlib-ng/zlib-ng.git")

#////////////////////////////////////#
#            IXWebSocket             #
#////////////////////////////////////#
set(IXWEBSOCKET_TAG "v11.4.6")
set(IXWEBSOCKET_REPOSITORY "https://gitlab.com/external-packages/ixwebsocket.git")

#////////////////////////////////////#
#            FlakedTuna              #
#////////////////////////////////////#
set(FLAKEDTUNA_TAG "1.1")
set(FLAKEDTUNA_REPOSITORY "https://github.com/flagarde/FlakedTuna.git")

#////////////////////////////////////#
#                Fmt                 #
#////////////////////////////////////#
set(FMT_TAG "12.1.0")
set(FMT_REPOSITORY "https://gitlab.com/external-packages/fmt.git")

#////////////////////////////////////#
#               Spdlog               #
#////////////////////////////////////#
set(SPDLOG_TAG "v1.17.0")
set(SPDLOG_REPOSITORY "https://gitlab.com/external-packages/spdlog.git")

#////////////////////////////////////#
#             magic_enum             #
#////////////////////////////////////#
set(MAGIC_ENUM_TAG "v0.9.7")
set(MAGIC_ENUM_REPOSITORY "https://gitlab.com/external-packages/magic-enum.git")

#////////////////////////////////////#
#              Jsoncpp               #
# TO REMOVE TO REMOVE TO REMOVE TO RE#
#////////////////////////////////////#
set(JSONCPP_TAG "1.9.5")
set(JSONCPP_REPOSITORY "https://github.com/open-source-parsers/jsoncpp.git")

#////////////////////////////////////#
#              nlohmann              #
#////////////////////////////////////#
set(NLOHMANN_TAG "v3.12.0")
set(NLOHMANN_REPOSITORY "https://gitlab.com/external-packages/json.git")

#////////////////////////////////////#
#            json-rpc-cxx            #
#////////////////////////////////////#
set(JSON_RPC_CXX_TAG "v0.3.0")
set(JSON_RPC_CXX_REPOSITORY "https://github.com/jsonrpcx/json-rpc-cxx.git")

#////////////////////////////////////#
#              Toml11                #
#////////////////////////////////////#
set(TOML11_TAG "v4.4.0")
set(TOML11_REPOSITORY "https://gitlab.com/external-packages/toml11.git")

#////////////////////////////////////#
#            dotenv-cpp              #
#////////////////////////////////////#
set(DOTENV_TAG "master")
set(DOTENV_REPOSITORY "https://gitlab.com/external-packages/dotenv-cpp.git")

#////////////////////////////////////#
#          source_location           #
#////////////////////////////////////#
set(SOURCELOCATION_TAG "main")
set(SOURCELOCATION_REPOSITORY "https://gitlab.com/external-packages/source-location.git")

######################################
#           EXTRA PACKAGES           #
######################################

#////////////////////////////////////#
#              elogpp                #
#////////////////////////////////////#
set(ELOGPP_TAG "main")
set(ELOGPP_REPOSITORY "https://github.com/flagarde/elogpp.git")

#////////////////////////////////////#
#             CAENlibs               #
#////////////////////////////////////#
set(CAENLIBS_REPOSITORY "https://gitlab.com/flagarde/CAENlibs.git")
set(CAENLIBS_TAG "main")

#////////////////////////////////////#
#               soci                 #
#////////////////////////////////////#
set(SOCI_TAG "master")
set(SOCI_REPOSITORY "https://gitlab.com/external-packages/soci.git")

#////////////////////////////////////#
#             net-snmp               #
#////////////////////////////////////#
set(NET-SNMP_TAG "v5.9.4")
set(NET-SNMP_REPOSITORY "https://github.com/net-snmp/net-snmp.git")

#////////////////////////////////////#
#              jsroot                #
#////////////////////////////////////#
set(JSROOT_TAG "6.2.1")
set(JSROOT_REPOSITORY "https://github.com/root-project/jsroot.git")

#////////////////////////////////////#
#           cpp-linenoise            #
#////////////////////////////////////#
set(CPP_LINENOISE_TAG "main")
set(CPP_LINENOISE_REPOSITORY "https://github.com/flagarde/cpp-linenoise.git")

set(serial_repository "https://gitlab.com/flagarde/serial")
set(serial_version "master")
