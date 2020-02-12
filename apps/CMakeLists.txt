if(BUILD_ANALYSIS)
  add_executable(Analysis Analysis.cpp)
  target_link_libraries(Analysis PRIVATE Event_static PRIVATE Channel_static)
  install(TARGETS Analysis RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
endif()

if(BUILD_WEBSOCKETSERVER)
  add_executable(WebsocketServer WebsocketServer.cpp)
  add_dependencies(WebsocketServer websocket CLI11)
  target_include_directories(WebsocketServer PRIVATE "${INCLUDE_OUTPUT_DIRECTORY}")
  target_link_libraries(WebsocketServer websocket)
  install(TARGETS WebsocketServer RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
endif()


if(NOT ${ANALYSER}) 
    add_executable(WaveDump WaveDump.cpp)
    
    target_include_directories(WaveDump PRIVATE ${ZLIB_INCLUDE_DIRS})
    add_dependencies(WaveDump lib1 lib2 web elogpp IXWebSocket CAENlibs)
    target_include_directories(WaveDump PUBLIC "${INCLUDE_OUTPUT_DIRECTORY}/ixwebsocket" PRIVATE "${INCLUDE_OUTPUT_DIRECTORY}/CAEN" PRIVATE "${INCLUDE_OUTPUT_DIRECTORY}/elogpp" PRIVATE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/CAEN> PRIVATE "${INCLUDE_OUTPUT_DIRECTORY}")
    target_link_libraries(WaveDump ${ROOT_LIBRARIES} CAENDigitizer CAENComm lib1 lib2 web  elogcpp jsoncpp m crypto Channel_static Event_static  ${ZLIB_LIBRARIES} mbedcrypto ixwebsocket mbedtls mbedx509)
    install(TARGETS WaveDump RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
    
    
    
    add_executable(Configurator Configurator.cpp)
    add_dependencies(Configurator websocket fmt CLI11 configurator)
    target_include_directories(Configurator PUBLIC "${INCLUDE_OUTPUT_DIRECTORY}/" PRIVATE "${CMAKE_SOURCE_DIR}/source/include"  )
    target_link_libraries(Configurator websocket  configurator ${CMAKE_DL_LIBS} ${DATABASES_LIBRARIES} ${MYSQL_LIBRARIES})
    install(TARGETS Configurator RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
    
    add_executable(DumpModule DumpModule.cpp)
    add_dependencies(DumpModule  websocket Configuration)
    target_include_directories(DumpModule PUBLIC "${INCLUDE_OUTPUT_DIRECTORY}/" PRIVATE "${CMAKE_SOURCE_DIR}/source/include")
    target_link_libraries(DumpModule websocket message Configuration)
    install(TARGETS DumpModule RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
    
    
      
    add_executable(Conf Conf.cpp)
    add_dependencies(Conf Configuration websocket)
        target_include_directories(Conf PUBLIC "${INCLUDE_OUTPUT_DIRECTORY}/toml" PRIVATE "${CMAKE_SOURCE_DIR}/source/include")
        target_link_libraries(Conf Configuration websocket)
    install(TARGETS Conf RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
    
endif()

