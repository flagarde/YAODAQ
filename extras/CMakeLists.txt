find_package(Git REQUIRED)

find_package(Threads)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

if(BUILD_DAQ)
  if(CAEN_HARDWARE)
    add_subdirectory(CAEN)
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
  endif()
endif()
