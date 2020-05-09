if(BUILD_ANALYSIS)
  add_executable(Analysis Analysis.cpp)
  target_link_libraries(Analysis PRIVATE Event_static PRIVATE Channel_static)
  install(TARGETS Analysis RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
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

add_executable(PluginTest PluginTest.cpp)
add_dependencies(PluginTest Board)
target_link_libraries(PluginTest Board)
install(TARGETS PluginTest RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})

if(BUILD_OLD_WAVEDUMP)
  include(elogpp)
  add_executable(WaveDump WaveDump.cpp)
  add_dependencies(WaveDump lib1 lib2 web elogpp)
  target_include_directories(WaveDump PRIVATE "${INCLUDE_OUTPUT_DIR}/CAEN" PRIVATE "${INCLUDE_OUTPUT_DIR}/elogpp" PRIVATE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/CAEN> PRIVATE "${INCLUDE_OUTPUT_DIR}")
  target_link_libraries(
    WaveDump
    lib1
    lib2
    web
    elogcpp
    Util
    m
    Channel_static
    Event_static
    CAENDigitizer
    CAENComm
    )
  install(TARGETS WaveDump RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})

  add_executable(DumpModule DumpModule.cpp)

  add_dependencies(DumpModule CAENBoards Board)
  target_include_directories(DumpModule PRIVATE ${CMAKE_BINARY_DIR} PRIVATE "${INCLUDE_OUTPUT_DIR}/CAEN")
  target_link_libraries(DumpModule PRIVATE CAENBoards PRIVATE Board)
  install(TARGETS DumpModule RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
endif()
