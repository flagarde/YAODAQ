include_guard(GLOBAL)

include(CPM)
cpm()

if(NOT DEFINED CPP_LINENOISE_REPOSITORY)
  set(CPP_LINENOISE_REPOSITORY "https://github.com/flagarde/cpp-linenoise.git")
endif()

if(NOT DEFINED CPP_LINENOISE_TAG)
  set(CPP_LINENOISE_TAG "main")
endif()

include(Jsoncpp)
include(OpenSSL)

declare_option(REPOSITORY cpp-linenoise OPTION BUILD_EXAMPLES VALUE OFF)
print_options(REPOSITORY  cpp-linenoise)

CPMAddPackage(NAME cpp-linenoise
              GIT_REPOSITORY ${CPP_LINENOISE_REPOSITORY}
              GIT_TAG ${CPP_LINENOISE_TAG}
              FETCHCONTENT_UPDATES_DISCONNECTED ${IS_OFFLINE}
              OPTIONS "${cpp-linenoise_OPTIONS}")
