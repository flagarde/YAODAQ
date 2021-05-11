include_guard(GLOBAL)

include(CPM)
cpm()

if(NOT DEFINED ELOGPP_REPOSITORY)
  set(ELOGPP_REPOSITORY "https://github.com/flagarde/elogpp")
endif()

if(NOT DEFINED ELOGPP_TAG)
  set(ELOGPP_TAG "main")
endif()

include(Jsoncpp)
include(OpenSSL)

declare_option(REPOSITORY elogpp OPTION COMPILE_ELOG VALUE OFF)
declare_option(REPOSITORY elogpp OPTION BUILD_TEST VALUE OFF)
declare_option(REPOSITORY elogpp OPTION USE_SYSTEM_OPENSSL VALUE OFF)
declare_option(REPOSITORY elogpp OPTION USE_SYSTEM_JSONCPP VALUE OFF)
print_options(REPOSITORY  elogpp)

CPMAddPackage(NAME elogpp
              GIT_REPOSITORY ${ELOGPP_REPOSITORY}
              GIT_TAG ${ELOGPP_TAG}
              FETCHCONTENT_UPDATES_DISCONNECTED ${IS_OFFLINE}
              OPTIONS "${elogpp_OPTIONS}")
