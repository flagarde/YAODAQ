include(CAENlibs)

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

add_library(CAENErrors STATIC "${CAEN_ERROR_SRC_FILES}")
add_dependencies(CAENErrors CAENlibs Error)
set_target_properties(CAENErrors PROPERTIES PUBLIC_HEADER "${CAEN_ERROR_INCLUDE_FILES}")
target_link_libraries(CAENErrors PUBLIC Error INTERFACE CAENVME INTERFACE CAENComm INTERFACE CAENDigitizer INTERFACE caenhvwrapper INTERFACE ${CMAKE_DL_LIBS} INTERFACE  Threads::Threads)
if(${ARCHITECTURE} STREQUAL "x64" OR ${ARCHITECTURE} STREQUAL "x86")
  target_link_libraries(CAENErrors INTERFACE ${DPP_LIB})
endif()
target_include_directories(CAENErrors PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/> PRIVATE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/CAEN> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIRECTORY}/CAEN>)
target_compile_definitions(CAENErrors PRIVATE ${CAEN_DEFS})
install(TARGETS CAENErrors RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION "include/CAEN")

set(CAEN_CONNECTOR_INCLUDE_FILES
    include/CAENVMEConnector.hpp
    include/CAENDigitizerConnector.hpp
    include/CAENHVConnector.hpp
   )

set(CAEN_CONNECTOR_SRC_FILES
    src/CAENVMEConnector.cpp
    src/CAENDigitizerConnector.cpp
    src/CAENHVConnector.cpp
   )

add_library(CAENConnectors SHARED "${CAEN_CONNECTOR_SRC_FILES}")
add_dependencies(CAENConnectors Connector CAENErrors)
set_target_properties(CAENConnectors PROPERTIES PUBLIC_HEADER "${CAEN_CONNECTOR_INCLUDE_FILES}")
target_link_libraries(CAENConnectors PUBLIC Connector PUBLIC CAENErrors)
target_include_directories(CAENConnectors PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/> PRIVATE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/CAEN> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIRECTORY}/CAEN>)
target_compile_definitions(CAENConnectors PRIVATE ${CAEN_DEFS})
install(TARGETS CAENConnectors RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION "include/CAEN")

set(CAENVME_INCLUDE_FILES
    include/CAENVMEBoard.hpp
   )

set(CAENVME_SRC_FILES
    src/CAENVMEBoard.cpp
   )
add_library(CAENVMEBoard SHARED "${CAENVME_SRC_FILES}")
add_dependencies(CAENVMEBoard Board)
set_target_properties(CAENVMEBoard PROPERTIES PUBLIC_HEADER "${CAEN_CONNECTOR_INCLUDE_FILES}")
target_link_libraries(CAENVMEBoard PUBLIC Board)
target_include_directories(CAENVMEBoard PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/> PRIVATE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/CAEN> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIRECTORY}/CAEN>)
target_compile_definitions(CAENVMEBoard PRIVATE ${CAEN_DEFS})
install(TARGETS CAENVMEBoard RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION "include/CAEN")

