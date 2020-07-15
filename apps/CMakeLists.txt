if(BUILD_ANALYSIS OR BUILD_OLD_WAVEDUMP)
  #find_package(Threads)
  #root_generate_dictionary(G__Channel "supress/Channel.hpp" LINKDEF "supress/ChannelLinkDef.hpp")
  #add_library(Channel OBJECT "supress/Channel.cpp" G__Channel.cxx)
  #add_dependencies(Channel ROOT)
  #target_include_directories(Channel PRIVATE "${YAODAQ_INCLUDE}")
  #target_link_libraries(Channel INTERFACE ROOT)
  #add_library(Channel_static STATIC $<TARGET_OBJECTS:Channel>)
  #target_link_libraries(Channel_static INTERFACE ROOT INTERFACE Threads::Threads)
  #target_include_directories(Channel_static PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)
  #add_custom_target(CopyChannel DEPENDS ALL COMMAND ${CMAKE_COMMAND} -E copy libChannel_rdict.pcm "${RUNTIME_OUTPUT_DIRECTORY}/libChannel_rdict.pcm")
  #set_target_properties(Channel PROPERTIES PUBLIC_HEADER "supress/Channel.hpp")
  #install(TARGETS Channel RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR} ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR} PUBLIC_HEADER DESTINATION include)
  #root_generate_dictionary(G__Event "supress/Event.hpp" LINKDEF "supress/EventLinkDef.hpp")
  #add_library(Event OBJECT "supress/Event.cpp" G__Event.cxx)
  #add_dependencies(Event ROOT)
  #target_include_directories(Event PRIVATE "${YAODAQ_INCLUDE}")
  #target_link_libraries(Event INTERFACE ROOT)
  #add_library(Event_static STATIC $<TARGET_OBJECTS:Event>)
  #target_include_directories(Event_static PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)
  #target_link_libraries(Event_static INTERFACE ROOT INTERFACE Threads::Threads)
  #add_custom_target(CopyEvent DEPENDS ALL COMMAND ${CMAKE_COMMAND} -E copy libEvent_rdict.pcm "${RUNTIME_OUTPUT_DIRECTORY}/libEvent_rdict.pcm")
  #set_target_properties(Event PROPERTIES PUBLIC_HEADER "supress/Event.hpp")
  #install(TARGETS Event RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR} ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR} PUBLIC_HEADER DESTINATION include)
endif()

if(BUILD_ANALYSIS)
  #add_executable(Analysis Analysis.cpp)
  #target_link_libraries(Analysis PRIVATE Event_static PRIVATE Channel_static)
  #target_include_directories(Analysis PRIVATE "supress")
  #install(TARGETS Analysis RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
endif()

if(BUILD_WEBSOCKETSERVER)
  add_executable(WebSocketServer WebsocketServer.cpp)
  add_dependencies(WebSocketServer WebsocketServer)
  target_link_libraries(WebSocketServer WebsocketServer)
  install(TARGETS WebSocketServer RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
endif()

if(BUILD_CONTROLLER)
  add_executable(Controller Controller.cpp)
  add_dependencies(Controller Controller_)
  target_link_libraries(Controller Controller_)
  install(TARGETS Controller RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
endif()

if(BUILD_LOGGER)
  add_executable(Logger Logger.cpp)
  add_dependencies(Logger Logger_)
  target_link_libraries(Logger Logger_)
  install(TARGETS Logger RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
endif()

if(BUILD_CONFIGURATOR)
  # add_executable(Configurator Configurator.cpp) add_dependencies(Configurator Configurator_) target_link_libraries(Configurator PUBLIC Configurator_) install(TARGETS Configurator RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
endif()

if(BUILD_OLD_WAVEDUMP)
  #include(elogpp)
  #add_executable(WaveDump WaveDump.cpp)

  #add_library(lib1 STATIC supress/Digitizer.cpp supress/flash.cpp supress/spi.cpp supress/WDconfig.cpp supress/X742CorrectionRoutines.cpp)
  #add_dependencies(lib1 CAENlibs)
  #target_include_directories(lib1 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/supress")
  #target_include_directories(lib1 PRIVATE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/CAEN> PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)

  #add_library(lib2 STATIC supress/FileManager.cpp)
  #add_dependencies(lib2 CAENlibs)
  #target_include_directories(lib2 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/supress")
  #target_include_directories(lib2 PRIVATE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/CAEN>)

  #add_library(web STATIC supress/Plotter.cpp supress/WebServer.cpp)
  #add_dependencies(web IXWebSocket)
  #target_link_libraries(web PUBLIC IXWebSocket)
  #target_include_directories(web PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/supress")
  #target_include_directories(web PRIVATE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/CAEN>)

  #add_dependencies(WaveDump lib1 lib2 web elogpp)
  #target_include_directories(WaveDump PRIVATE "${INCLUDE_OUTPUT_DIR}/CAEN" PRIVATE "${INCLUDE_OUTPUT_DIR}/elogpp" PRIVATE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/CAEN> PRIVATE "${INCLUDE_OUTPUT_DIR}")
  #target_link_libraries(
  #  WaveDump
  #  lib1
  #  lib2
  #  web
  #  elogcpp
  #  Util
  #  m
  #  Channel_static
  #  Event_static
  #  CAENDigitizer
  #  CAENComm
  #  )
  #install(TARGETS WaveDump RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
endif()

if(BUILD_DAQ)
  add_subdirectory(DAQ)
endif()
