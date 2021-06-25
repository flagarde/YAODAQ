include_guard(GLOBAL)

include(CPM)
cpm()

if(NOT DEFINED NLOHMANN_REPOSITORY)
    set(NLOHMANN_REPOSITORY "https://github.com/nlohmann/json.git")
endif()

if(NOT DEFINED NLOHMANN_TAG)
    set(NLOHMANN_TAG "v3.9.1")
endif()

declare_option(REPOSITORY nlohmann OPTION JSON_BuildTests VALUE OFF)
declare_option(REPOSITORY nlohmann OPTION JSON_Install VALUE ON)
declare_option(REPOSITORY nlohmann OPTION JSON_MultipleHeaders VALUE ON)
declare_option(REPOSITORY nlohmann OPTION JSON_ImplicitConversions VALUE ON)
declare_option(REPOSITORY nlohmann OPTION JSON_Diagnostics VALUE ON)
declare_option(REPOSITORY nlohmann OPTION JSON_CI VALUE OFF)
print_options(REPOSITORY nlohmann)

CPMAddPackage(NAME nlohmann
        GIT_REPOSITORY ${NLOHMANN_REPOSITORY}
        GIT_TAG ${NLOHMANN_TAG}
        FETCHCONTENT_UPDATES_DISCONNECTED ${IS_OFFLINE}
        OPTIONS "${nlohmann_OPTIONS}"
        )
