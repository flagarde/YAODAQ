if(NOT ${ANALYSER})     
    if(${ARCHITECTURE} STREQUAL "x64" OR ${ARCHITECTURE} STREQUAL "x86")
        set(DPPLIB CAENDPP)
        set(DPPSRC src/CAENDPPError.cpp)
    else()
        set(DPPSRC "")
        set(DPPLIB "")
    endif()
    
    message(STATUS ${DPPSRC})
    
    add_library(CAENError STATIC src/CAENVMEError.cpp src/CAENCommError.cpp ../src/Error.cpp ${DPPSRC} src/CAENDigitizerError.cpp src/CAENHVError.cpp src/CAENFlashError.cpp)
    add_dependencies(CAENError CAENlibs)
    target_include_directories(CAENError PUBLIC "${INCLUDE_OUTPUT_DIRECTORY}/CAEN" PUBLIC "${CMAKE_SOURCE_DIR}/source/include" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
    target_compile_definitions(CAENError PRIVATE  ${CAEN_DEFS})
    
endif()



