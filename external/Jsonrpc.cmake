include_guard(GLOBAL)

include(CPM)
cpm()

if(NOT DEFINED JSONRPC_REPOSITORY)
  set(JSONRPC_REPOSITORY "https://github.com/yaodaq/jsonrpc.git")
endif()

if(NOT DEFINED JSONRPC_TAG)
  set(JSONRPC_TAG "master")
endif()

declare_option(REPOSITORY jsonrpc OPTION FMT_PEDANTIC VALUE OFF)
print_options(REPOSITORY  jsonrpc)

CPMAddPackage(NAME jsonrpc
               GIT_REPOSITORY ${JSONRPC_REPOSITORY}
               GIT_TAG ${JSONRPC_TAG}
               FETCHCONTENT_UPDATES_DISCONNECTED ${IS_OFFLINE}
               OPTIONS "${jsonrpc_OPTIONS}"
              )
