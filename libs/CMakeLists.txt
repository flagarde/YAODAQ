find_package(Git REQUIRED)

find_package(Threads)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

set(YAODAQ_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(YAODAQ_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src")

if(BUILD_ANALYSIS OR BUILD_DAQ)
  root_generate_dictionary(G__Channel "${YAODAQ_INCLUDE}/Channel.hpp" LINKDEF
                           "${YAODAQ_INCLUDE}/ChannelLinkDef.hpp")
  add_library(Channel OBJECT "${YAODAQ_SRC}/Channel.cpp" G__Channel.cxx)
  target_include_directories(Channel PRIVATE "${YAODAQ_INCLUDE}")
  add_library(Channel_static STATIC $<TARGET_OBJECTS:Channel>)
  target_link_libraries(
    Channel_static
    INTERFACE ${ROOT_LIBRARIES}
    INTERFACE Threads::Threads)
  target_include_directories(
    Channel_static
    PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}>
    PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)
  add_custom_target(
    CopyChannel
    DEPENDS ALL
    COMMAND ${CMAKE_COMMAND} -E copy libChannel_rdict.pcm
            "${RUNTIME_OUTPUT_DIRECTORY}/libChannel_rdict.pcm")
  set_target_properties(Channel PROPERTIES PUBLIC_HEADER
                                           "${YAODAQ_INCLUDE}/Channel.hpp")
  install(
    TARGETS Channel
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR}
    ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR}
    PUBLIC_HEADER DESTINATION include)

  root_generate_dictionary(G__Event "${YAODAQ_INCLUDE}/Event.hpp" LINKDEF
                           "${YAODAQ_INCLUDE}/EventLinkDef.hpp")
  add_library(Event OBJECT "${YAODAQ_SRC}/Event.cpp" G__Event.cxx)
  target_include_directories(Event PRIVATE "${YAODAQ_INCLUDE}")
  target_link_libraries(Event INTERFACE ${ROOT_LIBRARIES})
  add_library(Event_static STATIC $<TARGET_OBJECTS:Event>)
  target_include_directories(
    Event_static
    PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}>
    PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)
  target_link_libraries(
    Event_static
    INTERFACE ${ROOT_LIBRARIES}
    INTERFACE Threads::Threads)
  add_custom_target(
    CopyEvent
    DEPENDS ALL
    COMMAND ${CMAKE_COMMAND} -E copy libEvent_rdict.pcm
            "${RUNTIME_OUTPUT_DIRECTORY}/libEvent_rdict.pcm")
  set_target_properties(Event PROPERTIES PUBLIC_HEADER
                                         "${YAODAQ_INCLUDE}/Event.hpp")
  install(
    TARGETS Event
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR}
    ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR}
    PUBLIC_HEADER DESTINATION include)
endif()

if(BUILD_DAQ OR BUILD_WEBSOCKETSERVER)

  configure_file("${YAODAQ_SRC}/ProgramInfos.hpp.in"
                 "${INCLUDE_OUTPUT_DIR}/ProgramInfos.hpp" @ONLY)

  include(IXWebSocket)
  add_library(WebSocket INTERFACE)
  add_dependencies(WebSocket IXWebSocket)
  if(NOT WIN32)
    find_package(ZLIB)
    target_link_libraries(
      WebSocket
      INTERFACE ZLIB::ZLIB
      INTERFACE Threads::Threads
      INTERFACE mbedcrypto
      INTERFACE ixwebsocket
      INTERFACE mbedtls
      INTERFACE mbedx509)
    target_include_directories(WebSocket
                               INTERFACE "${INCLUDE_OUTPUT_DIR}/ixwebsocket")
  else()
    target_link_libraries(
      WebSocket
      INTERFACE Threads::Threads
      INTERFACE mbedcrypto
      INTERFACE ixwebsocket
      INTERFACE mbedtls
      INTERFACE mbedx509)
    target_include_directories(WebSocket
                               INTERFACE "${INCLUDE_OUTPUT_DIR}/ixwebsocket")
  endif()

  include(CLI11)
  include(spdlog)
  include(magic_enum)
  add_library(Util INTERFACE)
  add_dependencies(Util CLI11 spdlog magic_enum)
  target_link_libraries(Util INTERFACE spdlog)
  target_include_directories(
    Util
    INTERFACE "${INCLUDE_OUTPUT_DIR}"
    INTERFACE "${INCLUDE_OUTPUT_DIR}/spdlog")

  include(elogpp)
  include(soci)
  include(toml11)

  add_library(Connectors INTERFACE)
  add_library(Boards INTERFACE)

  add_library(Exception "${YAODAQ_SRC}/Exception.cpp")
  add_dependencies(Exception Util)
  target_include_directories(
    Exception
    PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}>
    PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)
  set_target_properties(Exception PROPERTIES PUBLIC_HEADER
                                             "${YAODAQ_INCLUDE}/Exception.hpp")
  target_link_libraries(Exception PUBLIC Util)
  install(
    TARGETS Exception
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR}
    ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR}
    PUBLIC_HEADER DESTINATION include)

  add_library(Controller_ "${YAODAQ_SRC}/Controller.cpp")
  add_dependencies(Controller_ WebSocketClient Message Exception)
  target_link_libraries(
    Controller_
    PUBLIC WebSocketClient
    PUBLIC Message
    PUBLIC Exception)
  target_include_directories(
    Controller_
    PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}>
    PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>
    PUBLIC "${INCLUDE_OUTPUT_DIR}/toml")
  set_target_properties(
    Controller_ PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/Controller.hpp")
  install(
    TARGETS Controller_
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR}
    ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR}
    PUBLIC_HEADER DESTINATION include)

  add_library(ConnectorInfos "${YAODAQ_SRC}/ConnectorInfos.cpp")
  add_dependencies(ConnectorInfos toml11)
  target_include_directories(
    ConnectorInfos
    PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}>
    PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>
    PUBLIC "${INCLUDE_OUTPUT_DIR}/toml")
  set_target_properties(
    ConnectorInfos PROPERTIES PUBLIC_HEADER
                              "${YAODAQ_INCLUDE}/ConnectorInfos.hpp")
  install(
    TARGETS ConnectorInfos
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR}
    ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR}
    PUBLIC_HEADER DESTINATION include)

  add_library(Connector "${YAODAQ_SRC}/Connector.cpp")
  add_dependencies(Connector ConnectorInfos)
  target_include_directories(
    Connector
    PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}>
    PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)
  target_link_libraries(Connector PUBLIC ConnectorInfos)
  set_target_properties(Connector PROPERTIES PUBLIC_HEADER
                                             "${YAODAQ_INCLUDE}/Connector.hpp")
  install(
    TARGETS Connector
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR}
    ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR}
    PUBLIC_HEADER DESTINATION include)

  add_library(ConnectorFactory "${YAODAQ_SRC}/ConnectorFactory.cpp")
  add_dependencies(ConnectorFactory ConnectorInfos Connectors)
  target_include_directories(
    ConnectorFactory
    PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}>
    PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)
  target_link_libraries(
    ConnectorFactory
    PUBLIC ConnectorInfos
    PUBLIC Connectors)
  set_target_properties(
    ConnectorFactory PROPERTIES PUBLIC_HEADER
                                "${YAODAQ_INCLUDE}/ConnectorFactory.hpp")
  install(
    TARGETS ConnectorFactory
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR}
    ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR}
    PUBLIC_HEADER DESTINATION include)

  add_library(Infos "${YAODAQ_SRC}/Infos.cpp")
  add_dependencies(Infos Exception)
  target_link_libraries(Infos PUBLIC Exception)
  set_target_properties(Infos PROPERTIES PUBLIC_HEADER
                                         "${YAODAQ_INCLUDE}/Infos.hpp")
  target_include_directories(
    Infos
    PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}>
    PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)
  install(
    TARGETS Infos
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR}
    ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR}
    PUBLIC_HEADER DESTINATION include)

  add_library(BoardInfos "${YAODAQ_SRC}/BoardInfos.cpp")
  target_include_directories(
    BoardInfos
    PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}>
    PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>
    PUBLIC "${INCLUDE_OUTPUT_DIR}/toml")
  add_dependencies(BoardInfos toml11 Infos)
  target_link_libraries(BoardInfos PUBLIC Infos)
  set_target_properties(
    BoardInfos PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/BoardInfos.hpp")
  install(
    TARGETS BoardInfos
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR}
    ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR}
    PUBLIC_HEADER DESTINATION include)

  add_library(Configuration "${YAODAQ_SRC}/Configuration.cpp")
  add_dependencies(Configuration ConnectorInfos BoardInfos)
  target_include_directories(
    Configuration
    PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}>
    PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)
  target_link_libraries(
    Configuration
    PUBLIC ConnectorInfos
    PUBLIC BoardInfos)
  set_target_properties(
    Configuration PROPERTIES PUBLIC_HEADER
                             "${YAODAQ_INCLUDE}/Configuration.hpp")
  install(
    TARGETS Configuration
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR}
    PUBLIC_HEADER DESTINATION include)

  add_library(Message "${YAODAQ_SRC}/Message.cpp")
  add_dependencies(Message elogpp Exception Util)
  target_include_directories(
    Message
    PUBLIC "${INCLUDE_OUTPUT_DIR}/json"
    PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}>
    PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)
  set_target_properties(Message PROPERTIES PUBLIC_HEADER
                                           "${YAODAQ_INCLUDE}/Message.hpp")
  target_link_libraries(
    Message
    PUBLIC jsoncpp
    PUBLIC Exception)
  install(
    TARGETS Message
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR}
    ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR}
    PUBLIC_HEADER DESTINATION include)

  add_library(WebSocketClient "${YAODAQ_SRC}/WebsocketClient.cpp")
  add_dependencies(WebSocketClient WebSocket Util Exception)
  target_link_libraries(
    WebSocketClient
    PUBLIC WebSocket
    PUBLIC Util
    PUBLIC Exception)
  target_include_directories(
    WebSocketClient
    PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}>
    PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)
  set_target_properties(
    WebSocketClient PROPERTIES PUBLIC_HEADER
                               "${YAODAQ_INCLUDE}/WebsocketClient.hpp")
  install(
    TARGETS WebSocketClient
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR}
    ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR}
    PUBLIC_HEADER DESTINATION include)

  add_library(Board "${YAODAQ_SRC}/Module.cpp" "${YAODAQ_SRC}/Board.cpp")
  add_dependencies(Board Message Configuration ConnectorFactory WebSocketClient)
  target_link_libraries(
    Board
    PUBLIC Message
    PUBLIC WebSocketClient
    PUBLIC Configuration
    PUBLIC ConnectorFactory)
  set_target_properties(
    Board PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/Module.hpp"
                     PUBLIC_HEADER "${YAODAQ_INCLUDE}/Board.hpp")
  install(
    TARGETS Board
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR}
    ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR}
    PUBLIC_HEADER DESTINATION include)

  add_library(Logger_ "${YAODAQ_SRC}/Logger.cpp")
  add_dependencies(Logger_ Message WebSocketClient Exception Util)
  target_link_libraries(
    Logger_
    PUBLIC Message
    PUBLIC WebSocketClient
    PUBLIC Exception
    PUBLIC Util)
  set_target_properties(Logger_ PROPERTIES PUBLIC_HEADER
                                           "${YAODAQ_INCLUDE}/Logger.hpp")
  install(
    TARGETS Logger_
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR}
    ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR}
    PUBLIC_HEADER DESTINATION include)

  if(BUILD_WEBSOCKETSERVER)
    add_library(WebsocketServer "${YAODAQ_SRC}/WebsocketServer.cpp")
    add_dependencies(WebsocketServer Util Message Infos WebSocket Exception)
    target_include_directories(WebsocketServer
                               PRIVATE $<BUILD_INTERFACE:${YAODAQ_INCLUDE}>)
    set_target_properties(
      WebsocketServer PROPERTIES PUBLIC_HEADER
                                 "${YAODAQ_INCLUDE}/WebsocketServer.hpp")
    target_link_libraries(
      WebsocketServer
      PUBLIC Util
      PUBLIC Message
      PUBLIC Infos
      PUBLIC WebSocket
      PUBLIC Exception)

    if(BUILD_CONFIGURATOR)
      add_library(Configurator_ "${YAODAQ_SRC}/Configurator.cpp")
      add_dependencies(Configurator_ soci WebsocketServer Board)
      target_include_directories(
        Configurator_
        PRIVATE $<BUILD_INTERFACE:${YAODAQ_INCLUDE}>
        PUBLIC ${DATABASES_INCLUDE_DIR})
      set_target_properties(
        Configurator_ PROPERTIES PUBLIC_HEADER
                                 "${YAODAQ_INCLUDE}/Configurator.hpp")
      target_link_libraries(
        Configurator_
        PUBLIC Board
        PUBLIC WebsocketServer
        PUBLIC Soci)
    endif()

  endif()

  if(BUILD_OLD_WAVEDUMP)
   # add_library(
   #   lib1 STATIC src/flash.cpp src/WDconfig.cpp src/X742CorrectionRoutines.cpp
   
   #               src/spi.cpp src/Digitizer.cpp)
   # add_dependencies(lib1 Connectors)
   # target_link_libraries(lib1 PUBLIC Connectors)
   # target_include_directories(lib1
   #                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
   # target_include_directories(
   #   lib1
   #   PRIVATE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/CAEN>
   #   PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}>
   #   PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)

    #add_library(lib2 STATIC src/FileManager.cpp)
    #add_dependencies(lib2 Connectors)
    #target_link_libraries(lib2 PUBLIC Connectors)
    #target_include_directories(lib2
    #                           PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
    #target_include_directories(
    #  lib2 PRIVATE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/CAEN>)

    #add_library(web STATIC src/WebServer.cpp src/Plotter.cpp)
    #add_dependencies(web WebSocket Connectors)
    #target_link_libraries(
    #  web
    #  PUBLIC Connectors
    #  PUBLIC WebSocket)
    #target_include_directories(web
    
    #                           PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
    
    #target_include_directories(
    #  web PRIVATE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/CAEN>)
  endif()

endif()
