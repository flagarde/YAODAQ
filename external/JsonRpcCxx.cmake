include_guard(GLOBAL)

include(CPM)
cpm()

include(Nlohmann)

if(NOT DEFINED JSON_RPC_CXX_REPOSITORY)
  set(JSON_RPC_CXX_REPOSITORY "https://github.com/jsonrpcx/json-rpc-cxx.git")
endif()

if(NOT DEFINED JSON_RPC_CXX_TAG)
  set(JSON_RPC_CXX_TAG "v0.3.0")
endif()

declare_option(REPOSITORY json-rpc-cxx OPTION COMPILE_TESTS VALUE OFF)
declare_option(REPOSITORY json-rpc-cxx OPTION COMPILE_EXAMPLES VALUE OFF)
declare_option(REPOSITORY json-rpc-cxx OPTION CODE_COVERAGE VALUE OFF)
print_options(REPOSITORY  json-rpc-cxx)

CPMAddPackage(NAME json-rpc-cxx
               GIT_REPOSITORY ${JSON_RPC_CXX_REPOSITORY}
               GIT_TAG ${JSON_RPC_CXX_TAG}
               FETCHCONTENT_UPDATES_DISCONNECTED ${IS_OFFLINE}
               OPTIONS "${json-rpc-cxx_OPTIONS}"
              )
if(json-rpc-cxx_ADDED)
  target_link_libraries(json-rpc-cxx INTERFACE nlohmann_json::nlohmann_json)
  add_library(json-rpc-cxx::json-rpc-cxx ALIAS json-rpc-cxx)
endif()
