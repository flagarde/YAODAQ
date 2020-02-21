# IXWebSocket
set(IXWebSocket_repository "https://github.com/machinezone/IXWebSocket.git")
set(IXWebSocket_version "master")
# CAENlibs
set(CAENlibs_repository "https://github.com/RPClab/CAENlibs.git")
set(CAENlibs_version "master")
# elogpp
set(elogpp_repository "https://github.com/RPClab/elogpp.git")
set(elogpp_version "master")
# doctest
set(doctest_repository "https://github.com/onqtam/doctest.git")
set(doctest_version "master")
# fmt
set(fmt_repository "https://github.com/RPClab/fmt.git")
set(fmt_version "master")
# CLI11
set(CLI11_repository "https://github.com/RPClab/CLI11.git")
set(CLI11_version "master")
# soci
set(soci_repository "https://github.com/RPClab/soci.git")
set(soci_version "master")
# toml11
set(toml11_repository "https://github.com/RPClab/toml11.git")
set(toml11_version "master")
# spdlog
set(spdlog_repository "https://github.com/RPClab/spdlog.git")
set(spdlog_version "v1.x")
# civet
set(civet_repository "https://github.com/civetweb/civetweb.git")
set(civet_version "master")

option(ENABLE_TESTS "Include tests folder" ON)
option(ENABLE_DOCS "Include docs folder" ON)
option(BUILD_ANALYSIS "Build the Analysis code" ON)
option(BUILD_WEBSOCKETSERVER "Build the WebSocket Server" ON)
option(BUILD_WEBSERVER "Build the civet webserver" ON)
option(BUILD_DAQ "Build DAQ" ON)
option(BUILD_CONFIGURATOR "Build Configurator" ON)

option(BUILD_OLD_WAVEDUMP "Build the old wavedump to supress in the future" ON)

option(CAEN_HARDWARE "Compile CAEN Hardware library" ON)

option(ENABLE_ALL_WARNINGS "Compile with all warnings for the major compilers"
       ON)
option(ENABLE_EFFECTIVE_CXX "Enable Effective C++ warnings" OFF)
option(GENERATE_DEPENDENCY_DATA "Generates .d files with header dependencies"
       ON)

option(
  ENABLE_DOCTESTS
  "Include tests in the library. Setting this to OFF will remove all doctest related code.
                        Tests in tests/*.cpp will still be enabled."
  OFF)
