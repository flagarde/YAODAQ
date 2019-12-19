if(WIN32)
    message(STATUS "Found is Windows system")
    set(PLATFORM Windows)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES 64)
        message(STATUS "Found a 64bit system")
        set(ARCHITECTURE x64)
    else()
        message(STATUS "Found a 32bit system")
        set(ARCHITECTURE x86)
    endif()
elseif(APPLE)
    message(STATUS "Found is MacOS system")
    set(PLATFORM Unix)
    if (CMAKE_SYSTEM_NAME MATCHES Darwin)
        execute_process(COMMAND sw_vers "-productVersion"
                        COMMAND cut -d . -f 1-2
                        OUTPUT_VARIABLE MACOSX_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
        message(STATUS "Found a Mac OS X System ${MACOSX_VERSION}")
        if(MACOSX_VERSION VERSION_GREATER 10.4)
            if(CMAKE_SYSTEM_PROCESSOR MATCHES 64)
                message(STATUS "Found a 64bit system")
                set(ROOT_ARCHITECTURE x64)
            else()
                message(STATUS "Found a 32bit system")
                set(ROOT_ARCHITECTURE x86)
            endif()
        endif()
    endif()
elseif(UNIX)
    set(ARCHITECTURE Linux)
    set(PLATFORM Unix)
    message(STATUS "Found is Linux system")
    if(CMAKE_SYSTEM_PROCESSOR MATCHES x86_64)
        message(STATUS "Found a 64bit system")
        set(ARCHITECTURE x64)
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES aarch64)
        message(STATUS "Found a 64bit ARM system")
        set(ARCHITECTURE aarch64)
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES ppc64)
        message(STATUS "Found a 64bit PPC system (ppc64/ppc64le)")
        set(ARCHITECTURE ppc64)
        message(FATAL_ERROR "${CMAKE_SYSTEM_PROCESSOR} incompatible with CAEN libraries")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES arm)
        message(STATUS "Found a 32bit ARM system")
        set(ARCHITECTURE arm)
        message(FATAL_ERROR "${CMAKE_SYSTEM_PROCESSOR} incompatible with CAEN libraries")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES s390x)
        message(STATUS "Found a 64bit system")
        set(ARCHITECTURE s390x)
        message(FATAL_ERROR "${CMAKE_SYSTEM_PROCESSOR} incompatible with CAEN libraries")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES s390)
        message(STATUS "Found a 31bit system")
        set(ARCHITECTURE s390)
        message(FATAL_ERROR "${CMAKE_SYSTEM_PROCESSOR} incompatible with CAEN libraries")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES i686)
        message(STATUS "Found a 32bit system")
        set(ARCHITECTURE x86)
    else()
        message(FATAL_ERROR "Unknown processor: ${CMAKE_SYSTEM_PROCESSOR}")
    endif()
endif()

