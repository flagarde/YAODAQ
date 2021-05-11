include_guard(GLOBAL)

include(CPM)
cpm()

if(NOT DEFINED TOML11_REPOSITORY)
  set(TOML11_REPOSITORY "https://github.com/ToruNiina/toml11")
endif()

if(NOT DEFINED TOML11_TAG)
  set(TOML11_TAG "master")
endif()

declare_option(REPOSITORY toml11 OPTION toml11_BUILD_TEST VALUE OFF)
declare_option(REPOSITORY toml11 OPTION toml11_TEST_WITH_ASAN VALUE OFF)
declare_option(REPOSITORY toml11 OPTION toml11_TEST_WITH_UBSAN VALUE OFF)
print_options(REPOSITORY  toml11)

CPMAddPackage(NAME toml11
              GIT_REPOSITORY ${TOML11_REPOSITORY}
              GIT_TAG ${TOML11_TAG}
              FETCHCONTENT_UPDATES_DISCONNECTED ${IS_OFFLINE}
              OPTIONS "${toml11_OPTIONS}")
