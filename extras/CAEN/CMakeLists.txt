if(BUILD_DAQ OR CAEN_HARDWARE)
  include(CAENlibs)
  set(CAEN_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")
  set(CAEN_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src")
  
  ##CAEN Exceptions
  set(CAEN_EXCEPTION_INCLUDE_FILES
      ${CAEN_INCLUDE}/CAENCommException.hpp 
      ${CAEN_INCLUDE}/CAENDigitizerException.hpp
      ${CAEN_INCLUDE}/CAENFlashException.hpp 
      ${CAEN_INCLUDE}/CAENHVException.hpp
      ${CAEN_INCLUDE}/CAENVMEException.hpp 
      $<$<OR:$<STREQUAL:${ARCHITECTURE},x64>,$<STREQUAL:${ARCHITECTURE},x86>>:${CAEN_INCLUDE}/CAENDPPException.hpp>
     )
  set(CAEN_EXCEPTION_SRC_FILES
      ${CAEN_SRC}/CAENCommException.cpp 
      ${CAEN_SRC}/CAENDigitizerException.cpp
      ${CAEN_SRC}/CAENFlashException.cpp 
      ${CAEN_SRC}/CAENHVException.cpp 
      ${CAEN_SRC}/CAENVMEException.cpp
      $<$<OR:$<STREQUAL:${ARCHITECTURE},x64>,$<STREQUAL:${ARCHITECTURE},x86>>:${CAEN_SRC}/CAENDPPException.cpp>
     )
  add_library(CAENExceptions STATIC "${CAEN_EXCEPTION_SRC_FILES}")
  add_dependencies(CAENExceptions CAENlibs)
  set_target_properties(CAENExceptions PROPERTIES PUBLIC_HEADER "${CAEN_EXCEPTION_INCLUDE_FILES}")
  target_link_libraries(CAENExceptions
                        PUBLIC Exception
                        PUBLIC CAENVME
                        PUBLIC CAENComm
                        PUBLIC CAENDigitizer
                        PUBLIC caenhvwrapper
                        PUBLIC ${CMAKE_DL_LIBS}
                        PUBLIC Threads::Threads
                        PUBLIC $<$<OR:$<STREQUAL:${ARCHITECTURE},x64>,$<STREQUAL:${ARCHITECTURE},x86>>:CAENDPP>
                      )
  target_include_directories(CAENExceptions
                             PUBLIC $<BUILD_INTERFACE:${CAEN_INCLUDE}>
                             PRIVATE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/CAEN>
                             PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}/CAEN>
                            )
  target_compile_definitions(CAENExceptions PRIVATE ${CAEN_DEFS})
  install(TARGETS CAENExceptions
          RUNTIME DESTINATION "${RUNTIME_OUTPUT_DIR}"
          LIBRARY DESTINATION "${LIBRARY_OUTPUT_DIR}"
          ARCHIVE DESTINATION "${ARCHIVE_OUTPUT_DIR}"
          PUBLIC_HEADER DESTINATION "${INCLUDE_OUTPUT_DIR}/CAEN"
         )
  
  ##CAEN Connectors
  set(CAEN_CONNECTOR_INCLUDE_FILES
      ${CAEN_INCLUDE}/CAENVMEConnector.hpp 
      ${CAEN_INCLUDE}/CAENDigitizerConnector.hpp
      ${CAEN_INCLUDE}/CAENHVConnector.hpp
     )
  set(CAEN_CONNECTOR_SRC_FILES
      ${CAEN_SRC}/CAENVMEConnector.cpp 
      ${CAEN_SRC}/CAENDigitizerConnector.cpp
      ${CAEN_SRC}/CAENHVConnector.cpp
     )
  add_library(CAENConnectors SHARED "${CAEN_CONNECTOR_SRC_FILES}")
  add_dependencies(CAENConnectors Connector CAENExceptions)
  set_target_properties(CAENConnectors PROPERTIES PUBLIC_HEADER "${CAEN_CONNECTOR_INCLUDE_FILES}")
  target_link_libraries(CAENConnectors PRIVATE CAENExceptions PRIVATE Connector)
  target_include_directories(CAENConnectors PRIVATE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/CAEN>)
  target_compile_definitions(CAENConnectors PRIVATE ${CAEN_DEFS})
  install(TARGETS CAENConnectors
          RUNTIME DESTINATION "${RUNTIME_OUTPUT_DIR}"
          LIBRARY DESTINATION "${LIBRARY_OUTPUT_DIR}"
          ARCHIVE DESTINATION "${ARCHIVE_OUTPUT_DIR}"
          PUBLIC_HEADER DESTINATION "${INCLUDE_OUTPUT_DIR}/CAEN"
         )
  ##CAEN Boards
  set(CAENBOARDS_INCLUDE_FILES 
      ${CAEN_INCLUDE}/CAENVMEBoard.hpp 
      ${CAEN_INCLUDE}/CAENCommBoard.hpp 
      ${CAEN_INCLUDE}/CAENDigitizerBoard.hpp
     )
  set(CAENBOARDS_SRC_FILES 
      ${CAEN_SRC}/CAENVMEBoard.cpp 
      ${CAEN_SRC}/CAENCommBoard.cpp 
      ${CAEN_SRC}/CAENDigitizerBoard.cpp
     )
  add_library(CAENBoards SHARED "${CAENBOARDS_SRC_FILES}")
  set_target_properties(CAENBoards PROPERTIES PUBLIC_HEADER "${CAENBOARDS_INCLUDE_FILES}")
  add_dependencies(CAENBoards Board SPI CAENExceptions)
  target_link_libraries(CAENBoards PUBLIC CAENExceptions PRIVATE Board PRIVATE SPI)
  target_include_directories(CAENBoards 
                            PRIVATE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/CAEN>
                            )
  target_compile_definitions(CAENBoards PRIVATE ${CAEN_DEFS})
  install(TARGETS CAENBoards
          RUNTIME DESTINATION "${RUNTIME_OUTPUT_DIR}"
          LIBRARY DESTINATION "${LIBRARY_OUTPUT_DIR}"
          ARCHIVE DESTINATION "${ARCHIVE_OUTPUT_DIR}"
          PUBLIC_HEADER DESTINATION "${INCLUDE_OUTPUT_DIR}/CAEN"
         )     
  ##SPI
  add_library(SPI STATIC ${CAEN_SRC}/SPI.cpp)
  add_dependencies(SPI CAENExceptions Flash)
  set_target_properties(SPI PROPERTIES PUBLIC_HEADER ${CAEN_INCLUDE}/SPI.hpp)
  target_link_libraries(SPI PRIVATE Flash PRIVATE CAENExceptions)
  target_include_directories(SPI PRIVATE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/CAEN>)
  target_compile_definitions(SPI PRIVATE ${CAEN_DEFS})
  install(TARGETS SPI
          RUNTIME DESTINATION "${RUNTIME_OUTPUT_DIR}"
          LIBRARY DESTINATION "${LIBRARY_OUTPUT_DIR}"
          ARCHIVE DESTINATION "${ARCHIVE_OUTPUT_DIR}"
          PUBLIC_HEADER DESTINATION "${INCLUDE_OUTPUT_DIR}/CAEN"
         )  
  #Flash
  add_library(Flash STATIC ${CAEN_SRC}/Flash.cpp)
  add_dependencies(Flash CAENExceptions)
  set_target_properties(Flash PROPERTIES PUBLIC_HEADER ${CAEN_INCLUDE}/Flash.hpp)
  target_link_libraries(Flash PRIVATE CAENExceptions)
  target_include_directories(Flash PRIVATE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/CAEN>)
  target_compile_definitions(Flash PRIVATE ${CAEN_DEFS})
  install(TARGETS Flash
          RUNTIME DESTINATION "${RUNTIME_OUTPUT_DIR}"
          LIBRARY DESTINATION "${LIBRARY_OUTPUT_DIR}"
          ARCHIVE DESTINATION "${ARCHIVE_OUTPUT_DIR}"
          PUBLIC_HEADER DESTINATION "${INCLUDE_OUTPUT_DIR}/CAEN"
         )  
 
 plugin(TARGET CAENConnectors VERSION 0.1 BASE Connector CHILDS CAEN::CAENVMEConnector CAEN::CAENDigitizerConnector CAEN::CAENHVConnector)  
 plugin(TARGET CAENBoards VERSION 0.1 BASE Board CHILDS CAEN::CAENVMEBoard CAEN::CAENDigitizerBoard CAEN::CAENCommBoard)

endif()
