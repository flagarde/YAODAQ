include_guard(GLOBAL)

include(CPM)
cpm()

if(NOT DEFINED CAENLIBS_REPOSITORY)
  set(CAENLIBS_REPOSITORY "https://github.com/flagarde/CAENlibs.git")
endif()

if(NOT DEFINED CAENLIBS_TAG)
  set(CAENLIBS_TAG "main")
endif()

declare_option(REPOSITORY CAENlibs OPTION BUILD_TESTS VALUE OFF)
print_options(REPOSITORY  CAENlibs)

CPMAddPackage(NAME CAENlibs
              GIT_REPOSITORY ${CAENLIBS_REPOSITORY}
              GIT_TAG ${CAENLIBS_TAG}
              FETCHCONTENT_UPDATES_DISCONNECTED ${IS_OFFLINE}
              OPTIONS "${CAENlibs_OPTIONS}")
