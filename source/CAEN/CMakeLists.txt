if(${ARCHITECTURE} STREQUAL "x64" OR ${ARCHITECTURE} STREQUAL "x86")
  set(DPP_LIB CAENDPP)
  set(DPP_SRC src/CAENDPPError.cpp)
  set(DPP_HEADER include/CAENDPPError.hpp)
else()
  set(DPPSRC "")
  set(DPPLIB "")
endif()

set(CAEN_ERROR_INCLUDE_FILES
    include/CAENCommError.hpp
    include/CAENDigitizerError.hpp
    include/CAENFlashError.hpp
    include/CAENHVError.hpp
    include/CAENVMEError.hpp
     ${DPP_HEADER}
    )

set(CAEN_ERROR_SRC_FILES
    src/CAENCommError.cpp
    src/CAENDigitizerError.cpp
    src/CAENFlashError.cpp
    src/CAENHVError.cpp
    src/CAENVMEError.cpp
    ${DPP_SRC}
    )

add_library(CAENError STATIC "${CAEN_ERROR_SRC_FILES}")
add_dependencies(CAENError CAENlibs Error)
set_target_properties(CAENError PROPERTIES PUBLIC_HEADER "${CAEN_ERROR_INCLUDE_FILES}")
target_link_libraries(CAENError PUBLIC Error INTERFACE CAENVME INTERFACE CAENComm INTERFACE CAENDigitizer INTERFACE caenhvwrapper INTERFACE ${CMAKE_DL_LIBS})
if(${ARCHITECTURE} STREQUAL "x64" OR ${ARCHITECTURE} STREQUAL "x86")
  target_link_libraries(CAENError INTERFACE ${DPP_LIB})
endif()
target_include_directories(CAENError PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/> PRIVATE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/CAEN> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIRECTORY}/CAEN>)
target_compile_definitions(CAENError PRIVATE ${CAEN_DEFS})
install(TARGETS CAENError RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION "include/CAEN")

set(CAEN_CONNECTOR_INCLUDE_FILES
    include/CAENVMEConnector.hpp
    )

set(CAEN_CONNECTOR_SRC_FILES
    src/CAENVMEConnector.cpp
    )

add_library(CAENConnectors STATIC "${CAEN_CONNECTOR_SRC_FILES}")
add_dependencies(CAENConnectors Connector)
set_target_properties(CAENConnectors PROPERTIES PUBLIC_HEADER "${CAEN_CONNECTOR_INCLUDE_FILES}")
target_link_libraries(CAENConnectors PUBLIC Connector)
target_include_directories(CAENConnectors PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/> PRIVATE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/CAEN> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIRECTORY}/CAEN>)
target_compile_definitions(CAENConnectors PRIVATE ${CAEN_DEFS})
install(TARGETS CAENConnectors RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION "include/CAEN")



