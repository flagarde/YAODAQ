if(BUILD_DAQ OR CAEN_HARDWARE)
    add_dependencies(Connectors CAENlibs)
    target_link_libraries(Connectors INTERFACE CAENConnectors)
    target_include_directories(
      Connectors
      INTERFACE $<BUILD_INTERFACE:${YAODAQ_SRC}/CAEN/include>
      INTERFACE $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}/CAEN>)
    target_compile_definitions(Connectors INTERFACE CAENLIBS)
    add_dependencies(Boards CAENBoard)
    target_link_libraries(Boards INTERFACE CAENBoard)
    target_include_directories(
      Boards
      INTERFACE $<BUILD_INTERFACE:${YAODAQ_SRC}/CAEN/include>
      INTERFACE $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}/CAEN>)
    target_compile_definitions(Boards INTERFACE CAENLIBS)

include(CAENlibs)

if(${ARCHITECTURE} STREQUAL "x64" OR ${ARCHITECTURE} STREQUAL "x86")
  set(DPP_LIB CAENDPP)
  set(DPP_SRC src/CAENDPPException.cpp)
  set(DPP_HEADER include/CAENDPPException.hpp)
else()
  set(DPPSRC "")
  set(DPPLIB "")
endif()

set(CAEN_EXCEPTION_INCLUDE_FILES
    include/CAENCommException.hpp include/CAENDigitizerException.hpp
    include/CAENFlashException.hpp include/CAENHVException.hpp
    include/CAENVMEException.hpp ${DPP_HEADER})

set(CAEN_EXCEPTION_SRC_FILES
    src/CAENCommException.cpp src/CAENDigitizerException.cpp
    src/CAENFlashException.cpp src/CAENHVException.cpp src/CAENVMEException.cpp
    ${DPP_SRC})

add_library(CAENExceptions STATIC "${CAEN_EXCEPTION_SRC_FILES}")
add_dependencies(CAENExceptions CAENlibs Exception)
set_target_properties(
  CAENExceptions PROPERTIES PUBLIC_HEADER "${CAEN_EXCEPTION_INCLUDE_FILES}")
target_link_libraries(
  CAENExceptions
  PUBLIC Exception
  INTERFACE CAENVME
  INTERFACE CAENComm
  INTERFACE CAENDigitizer
  INTERFACE caenhvwrapper
  INTERFACE ${CMAKE_DL_LIBS}
  INTERFACE Threads::Threads)
if(${ARCHITECTURE} STREQUAL "x64" OR ${ARCHITECTURE} STREQUAL "x86")
  target_link_libraries(CAENExceptions INTERFACE ${DPP_LIB})
endif()
target_include_directories(
  CAENExceptions
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
  PRIVATE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/CAEN>
  PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIRECTORY}/CAEN>)
target_compile_definitions(CAENExceptions PRIVATE ${CAEN_DEFS})
install(
  TARGETS CAENExceptions
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR}
  PUBLIC_HEADER DESTINATION "include/CAEN")

set(CAEN_CONNECTOR_INCLUDE_FILES
    include/CAENVMEConnector.hpp include/CAENDigitizerConnector.hpp
    include/CAENHVConnector.hpp)

set(CAEN_CONNECTOR_SRC_FILES
    src/CAENVMEConnector.cpp src/CAENDigitizerConnector.cpp
    src/CAENHVConnector.cpp)

add_library(CAENConnectors SHARED "${CAEN_CONNECTOR_SRC_FILES}")
add_dependencies(CAENConnectors Connector CAENExceptions)
set_target_properties(
  CAENConnectors PROPERTIES PUBLIC_HEADER "${CAEN_CONNECTOR_INCLUDE_FILES}")
target_link_libraries(
  CAENConnectors
  PUBLIC Connector
  PUBLIC CAENExceptions)
target_include_directories(
  CAENConnectors
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
  PRIVATE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/CAEN>
  PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}/CAEN>)
target_compile_definitions(CAENConnectors PRIVATE ${CAEN_DEFS})
install(
  TARGETS CAENConnectors
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR}
  PUBLIC_HEADER DESTINATION "include/CAEN")

set(CAENVME_INCLUDE_FILES include/CAENVMEBoard.hpp include/CAENCommBoard.hpp
                          include/CAENDigitizerBoard.hpp)

set(CAENVME_SRC_FILES src/CAENVMEBoard.cpp src/CAENCommBoard.cpp
                      src/CAENDigitizerBoard.cpp)
add_library(CAENBoard SHARED "${CAENVME_SRC_FILES}")
add_dependencies(CAENBoard Board SPI)
set_target_properties(CAENBoard PROPERTIES PUBLIC_HEADER
                                           "${CAEN_CONNECTOR_INCLUDE_FILES}")
target_link_libraries(
  CAENBoard
  PUBLIC Board
  PUBLIC SPI)
target_include_directories(
  CAENBoard
  PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
  PRIVATE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/CAEN>
  PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}/CAEN>)
target_compile_definitions(CAENBoard PRIVATE ${CAEN_DEFS})
install(
  TARGETS CAENBoard
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR}
  PUBLIC_HEADER DESTINATION "include/CAEN")

add_library(SPI STATIC src/SPI.cpp)
add_dependencies(SPI CAENExceptions)
set_target_properties(SPI PROPERTIES PUBLIC_HEADER include/SPI.hpp)
target_link_libraries(SPI PRIVATE Flash)
target_include_directories(
  SPI
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
  PRIVATE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/CAEN>
  PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}/CAEN>)
target_compile_definitions(SPI PRIVATE ${CAEN_DEFS})
install(
  TARGETS SPI
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR}
  PUBLIC_HEADER DESTINATION "include/CAEN")

add_library(Flash STATIC src/Flash.cpp)
add_dependencies(Flash CAENExceptions)
set_target_properties(Flash PROPERTIES PUBLIC_HEADER include/Flash.hpp)
target_link_libraries(Flash PUBLIC CAENExceptions)
target_include_directories(
  Flash
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
  PRIVATE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/CAEN>
  PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}/CAEN>)
target_compile_definitions(Flash PRIVATE ${CAEN_DEFS})
install(
  TARGETS Flash
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR}
  PUBLIC_HEADER DESTINATION "include/CAEN")
endif()
