####################################
# Options for the external package #
####################################

# OpenSSL
set(USE_SYSTEM_OPENSSL OFF)
set(OPENSSLCMAKE_REPOSITORY "https://gitlab.com/flagarde/openssl-cmake.git")
set(OPENSSLCMAKE_TAG "v2.1")
set(OPENSSL_VERSION "1.1.1k")
set(OPENSSL_URL "https://www.openssl.org/source/openssl-${OPENSSL_VERSION}.tar.gz")
set(OPENSSL_URL_HASH "SHA256=892a0875b9872acd04a9fde79b1f943075d5ea162415de3047c327df33fbaee5")

# Zlib-ng
set(USE_SYSTEM_ZLIB OFF)
set(ZLIB-NG_REPOSITORY "https://gitlab.com/ExternalRepositories/zlib-ng.git")
set(ZLIB-NG_TAG "2.0.3")

# IXWebSocket
set(IXWEBSOCKET_REPOSITORY "https://gitlab.com/ExternalRepositories/IXWebSocket.git")
set(IXWEBSOCKET_TAG "master")

# fmt
set(FMT_REPOSITORY "https://gitlab.com/ExternalRepositories/fmt")
set(FMT_VERSION "7.1.3")

# spdlog
set(SPDLOG_REPOSITORY "https://gitlab.com/ExternalRepositories/spdlog")
set(SPDLOG_VERSION "v1.8.5")





## General options
set(ENABLE_TESTS TRUE)

## INCLUDED BY CPM
# doctest
set(DOCTEST_REPOSITORY "https://gitlab.com/ExternalRepositories/doctest")
set(DOCTEST_VERSION "2.4.5")

# FlakedTuna
set(FLAKEDTUNA_REPOSITORY "https://gitlab.com/flagarde/FlakedTuna")
set(FLAKEDTUNA_VERSION "1.0")

# CLI11
set(CLI11_REPOSITORY "https://gitlab.com/ExternalRepositories/CLI11")
set(CLI11_TAG "master")


set(TOML11_REPOSITORY "https://gitlab.com/ExternalRepositories/toml11.git")
set(TOML11_TAG "master")

##




# jsoncpp
set(JSONCPP_REPOSITORY "https://gitlab.com/ExternalRepositories/jsoncpp")
set(JSONCPP_TAG "master")
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
# toml11
set(toml11_repository "https://gitlab.com/ExternalRepositories/toml11")
set(toml11_version "master")

# civet
set(civet_repository "https://github.com/civetweb/civetweb.git")
set(civet_version "master")
# magic_enum
set(magic_enum_repository "https://gitlab.com/ExternalRepositories/magic_enum")
set(magic_enum_version "master")
# jsroot
set(JSROOT_REPOSITORY "https://gitlab.com/ExternalRepositories/jsroot.git")
set(JSROOT_TAG "6.1.0")

# cereal
set(cereal_repository "https://github.com/USCiLab/cereal.git")
set(cereal_version "master")

set(SNMP_repository "https://github.com/RPClab/net-snmp.git")
set(SNMP_version "master")




# Compiling ROOT can be very long so :
option(USE_ROOT_SYSTEM "Try to find a ROOT installation and use it" ON)
option(BUILD_ROOT "Build ROOT Folder" ON)
set(LOOK_FOR_ROOT_VERSION "6.22.06")
# If it fails to find ROOT V${LOOK_FOR_ROOT_VERSION} or greater then Download and Install it !
set(ROOT_repository "https://root.cern/download/root_v${LOOK_FOR_ROOT_VERSION}.source.tar.gz")
# ########################################################################################################################################################################################################################################################

option(ENABLE_TESTS "Include tests folder" ON)
option(ENABLE_DOCS "Include docs folder" ON)
option(BUILD_ANALYSIS "Build the Analysis code" ON)
option(BUILD_WEBSOCKETSERVER "Build the WebSocket Server" ON)
option(BUILD_WEBSERVER "Build the civet webserver" ON)
option(BUILD_CONFIGURATOR "Build Configurator" ON)
option(BUILD_CONTROLLER "Build Controller" ON)
option(BUILD_LOGGER "Build Logger" ON)
option(ENABLE_EXTRAS "Build extras" ON)

option(BUILD_DAQ "Build DAQ" ON)
option(BUILD_SNMP "Build SNMP" OFF)


option(BUILD_LCIO "Build LCIO" OFF)

option(BUILD_OLD_WAVEDUMP "Build the old wavedump to supress in the future" ON)

option(CAEN_HARDWARE "Compile CAEN Hardware library" ON)

option(ENABLE_ALL_WARNINGS "Compile with all warnings for the major compilers" ON)
option(ENABLE_EFFECTIVE_CXX "Enable Effective C++ warnings" OFF)
option(GENERATE_DEPENDENCY_DATA "Generates .d files with header dependencies" ON)

option(ENABLE_DOCTESTS "Include tests in the library. Setting this to OFF will remove all doctest related code.
                        Tests in tests/*.cpp will still be enabled." OFF
       )
