# Options for the external package
# PMM
set(PMM_PATH "https://flagarde.github.io/pmm/pmm.cmake")
set(PMM_SHA256 "d7e4345d139bdb449a39c1fcf04b79f225569fbea6be6e3d0c342f9edeb3115e")
# fmt
set(FMT_REPOSITORY "https://github.com/fmtlib/fmt.git")
set(FMT_VERSION "master")
# spdlog
set(SPDLOG_REPOSITORY "https://github.com/gabime/spdlog.git")
set(SPDLOG_VERSION "v1.x")
# doctest
set(DOCTEST_REPOSITORY "https://github.com/onqtam/doctest.git")
set(DOCTEST_VERSION "master")









# IXWebSocket
set(IXWebSocket_repository "https://github.com/flagarde/IXWebSocket.git")
set(IXWebSocket_version "master")
# CAENlibs
set(CAENlibs_repository "https://github.com/RPClab/CAENlibs.git")
set(CAENlibs_version "master")
# elogpp
set(elogpp_repository "https://github.com/RPClab/elogpp.git")
set(elogpp_version "master")


# CLI11
set(CLI11_repository "https://github.com/RPClab/CLI11.git")
set(CLI11_version "master")
# soci
set(soci_repository "https://github.com/SOCI/soci.git")
set(soci_version "master")
# toml11
set(toml11_repository "https://github.com/RPClab/toml11.git")
set(toml11_version "master")

# civet
set(civet_repository "https://github.com/civetweb/civetweb.git")
set(civet_version "master")
# magic_enum
set(magic_enum_repository "https://github.com/Neargye/magic_enum.git")
set(magic_enum_version "master")
# jsroot
set(jsroot_repository "https://github.com/root-project/jsroot.git")
set(jsroot_version "master")
# jsoncpp
set(jsoncpp_repository "https://github.com/open-source-parsers/jsoncpp.git")
set(jsoncpp_version "master")
# cereal 
set(cereal_repository "https://github.com/USCiLab/cereal.git")
set(cereal_version "master")

set(SNMP_repository "https://github.com/RPClab/net-snmp.git" )
set(SNMP_version "master" )

## Compiling ROOT can be very long so :
option(USE_ROOT "Try to find a ROOT installation and use it" ON)
option(BUILD_ROOT "Build ROOT Folder" ON)
set(LOOK_FOR_ROOT_VERSION "6.20.04")
## If it fails to find ROOT V${LOOK_FOR_ROOT_VERSION} or greater then Download and Install it !
set(ROOT_repository "https://root.cern/download/root_v${LOOK_FOR_ROOT_VERSION}.source.tar.gz")
#######################################



option(ENABLE_TESTS "Include tests folder" OFF)
option(ENABLE_DOCS "Include docs folder" ON)
option(BUILD_ANALYSIS "Build the Analysis code" ON)
option(BUILD_WEBSOCKETSERVER "Build the WebSocket Server" ON)
option(BUILD_WEBSERVER "Build the civet webserver" ON)
option(BUILD_CONFIGURATOR "Build Configurator" ON)
option(BUILD_CONTROLLER "Build Controller" ON)
option(BUILD_LOGGER "Build Logger" ON)
option(ENABLE_EXTRAS "Build extras" ON)


option(BUILD_DUMP "Build DUMP" ON)
option(BUILD_SNMP "Build SNMP" OFF)

option(BUILD_OLD_WAVEDUMP "Build the old wavedump to supress in the future" ON)

option(CAEN_HARDWARE "Compile CAEN Hardware library" ON)

option(ENABLE_ALL_WARNINGS "Compile with all warnings for the major compilers" ON)
option(ENABLE_EFFECTIVE_CXX "Enable Effective C++ warnings" OFF)
option(GENERATE_DEPENDENCY_DATA "Generates .d files with header dependencies" ON)

option(ENABLE_DOCTESTS "Include tests in the library. Setting this to OFF will remove all doctest related code.
                        Tests in tests/*.cpp will still be enabled." OFF
       )
