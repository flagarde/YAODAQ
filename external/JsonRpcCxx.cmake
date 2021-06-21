include_guard(GLOBAL)

include(CPM)
cpm()

if(NOT DEFINED JSON_RPC_CXX_REPOSITORY)
  set(JSON_RPC_CXX_REPOSITORY "https://gitlab.com/ExternalRepositories/json-rpc-cxx.git")
endif()

if(NOT DEFINED JSON_RPC_CXX_TAG)
  set(JSON_RPC_CXX_TAG "master")
endif()

declare_option(REPOSITORY json-rpx-cxx OPTION COMPILE_TESTS VALUE OFF)
declare_option(REPOSITORY json-rpx-cxx OPTION COMPILE_EXAMPLES VALUE OFF)
declare_option(REPOSITORY json-rpx-cxx OPTION CODE_COVERAGE VALUE OFF)

print_options(REPOSITORY  json-rpx-cxx)

CPMAddPackage(NAME json-rpx-cxx
               GIT_REPOSITORY ${JSON_RPC_CXX_REPOSITORY}
               GIT_TAG ${JSON_RPC_CXX_TAG}
               FETCHCONTENT_UPDATES_DISCONNECTED ${IS_OFFLINE}
               OPTIONS "${json-rpx-cxx_OPTIONS}"
              )
