include_guard(GLOBAL)

include(CPM)
cpm()

if(NOT DEFINED IXWEBSOCKET_REPOSITORY)
  set(IXWEBSOCKET_REPOSITORY "https://github.com/machinezone/IXWebSocket")
endif()

if(NOT DEFINED IXWEBSOCKET_TAG)
  set(IXWEBSOCKET_TAG "master")
endif()

include(Zlib-ng)
include(OpenSSL)

declare_option(REPOSITORY IXWebSocket OPTION USE_TLS VALUE ON)
declare_option(REPOSITORY IXWebSocket OPTION USE_OPEN_SSL VALUE ON)
declare_option(REPOSITORY IXWebSocket OPTION USE_ZLIB VALUE ON)
declare_option(REPOSITORY IXWebSocket OPTION USE_WS VALUE OFF)
declare_option(REPOSITORY IXWebSocket OPTION USE_TEST VALUE OFF)
declare_option(REPOSITORY IXWebSocket OPTION BUILD_DEMO VALUE OFF)
print_options(REPOSITORY  IXWebSocket)

CPMAddPackage(NAME IXWebSocket
              GIT_REPOSITORY ${IXWEBSOCKET_REPOSITORY}
              GIT_TAG ${IXWEBSOCKET_TAG}
              FETCHCONTENT_UPDATES_DISCONNECTED ${IS_OFFLINE}
              OPTIONS "${IXWebSocket_OPTIONS}")

if(IXWebSocket_ADDED)
  add_library(ixwebsocket::ixwebsocket ALIAS ixwebsocket)
endif()
