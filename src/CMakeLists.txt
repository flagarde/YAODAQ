set(YAODAQ_INCLUDE "${CMAKE_SOURCE_DIR}/include")

include(IXWebSocket)
include(FlakedTuna)
include(Spdlog)
include(MagicEnum)
include(Jsoncpp)
include(Toml11)
include(elogpp)


configure_file("ProgramInfos.hpp.in" "ProgramInfos.hpp" @ONLY)
add_library(ProgramInfos INTERFACE)
set_target_properties(ProgramInfos PROPERTIES PUBLIC_HEADER "${CMAKE_CURRENT_BINARY_DIR}/ProgramInfos.hpp")
target_link_libraries(ProgramInfos INTERFACE fmt::fmt)
target_include_directories(ProgramInfos INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> INTERFACE $<INSTALL_INTERFACE:${CMAKE_INSTALL_FULL_INCLUDEDIR}>)
install(TARGETS ProgramInfos)

add_library(Identifier "Identifier.cpp")
target_include_directories(Identifier PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
set_target_properties(Identifier PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/Identifier.hpp;${YAODAQ_INCLUDE}/Classes.hpp")
target_link_libraries(Identifier PUBLIC magic_enum::magic_enum)
install(TARGETS Identifier)

add_library(LoggerHandler "LoggerHandler.cpp")
target_include_directories(LoggerHandler PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
set_target_properties(LoggerHandler PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/LoggerHandler.hpp")
target_link_libraries(LoggerHandler PUBLIC spdlog::spdlog PRIVATE Identifier)
install(TARGETS LoggerHandler)

add_library(Message "Message.cpp")
target_include_directories(Message PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> PRIVATE $<BUILD_INTERFACE:${jsoncpp_SOURCE_DIR}/include>)
set_target_properties(Message PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/Message.hpp")
target_link_libraries(Message PUBLIC Exception PRIVATE jsoncpp_static PUBLIC magic_enum::magic_enum)
install(TARGETS Message)

add_library(MessageHandler "MessageHandler.cpp")
target_include_directories(MessageHandler PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
set_target_properties(MessageHandler PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/MessageHandler.hpp")
target_link_libraries(MessageHandler PUBLIC LoggerHandler PUBLIC Message PUBLIC Identifier)
install(TARGETS MessageHandler)








add_library(MessageHandlerServer "MessageHandlerServer.cpp")
target_include_directories(MessageHandlerServer PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
set_target_properties(MessageHandlerServer PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/MessageHandlerServer.hpp")
target_link_libraries(MessageHandlerServer PUBLIC MessageHandler PUBLIC ixwebsocket::ixwebsocket )
install(TARGETS MessageHandlerServer)

add_library(MessageHandlerClient "MessageHandlerClient.cpp")
target_include_directories(MessageHandlerClient PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
set_target_properties(MessageHandlerClient PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/MessageHandlerClient.hpp")
target_link_libraries(MessageHandlerClient PUBLIC MessageHandler PUBLIC ixwebsocket::ixwebsocket )
install(TARGETS MessageHandlerClient)




add_library(GeneralParameters "GeneralParameters.cpp")
target_include_directories(GeneralParameters PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
set_target_properties(GeneralParameters PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/GeneralParameters.hpp")
install(TARGETS GeneralParameters)

add_library(SourceLocation "SourceLocation.cpp")
target_include_directories(SourceLocation PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
set_target_properties(SourceLocation PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/SourceLocation.hpp")
install(TARGETS SourceLocation)

add_library(Exception "Exception.cpp")
target_include_directories(Exception PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
set_target_properties(Exception PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/Exception.hpp")
target_link_libraries(Exception PUBLIC fmt::fmt PRIVATE SourceLocation)
install(TARGETS Exception)



add_library(Key "Key.cpp")
target_include_directories(Key PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
set_target_properties(Key PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/Key.hpp")
target_link_libraries(Key PRIVATE SourceLocation PRIVATE fmt::fmt)
install(TARGETS Key)

add_library(Interrupt "Interrupt.cpp")
target_include_directories(Interrupt PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
set_target_properties(Interrupt PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/Interrupt.hpp")
target_link_libraries(Interrupt PUBLIC spdlog::spdlog PRIVATE SourceLocation)
install(TARGETS Interrupt)

add_library(File "File.cpp")
target_include_directories(File PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
set_target_properties(File PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/File.hpp")
target_link_libraries(File PUBLIC Message)
install(TARGETS File)

add_library(Controller "Controller.cpp")
target_link_libraries(Controller PUBLIC Board)
target_include_directories(Controller PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
set_target_properties(Controller PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/Controller.hpp")
install(TARGETS Controller)

add_library(ConnectorInfos "ConnectorInfos.cpp")
target_include_directories(ConnectorInfos PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
set_target_properties(ConnectorInfos PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/ConnectorInfos.hpp")
target_link_libraries(ConnectorInfos PUBLIC Exception PUBLIC toml11::toml11)
install(TARGETS ConnectorInfos)

add_library(Connector "Connector.cpp")
target_include_directories(Connector PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> PUBLIC "${jsoncpp_SOURCE_DIR}/include/json")
target_link_libraries(Connector PUBLIC ConnectorInfos PUBLIC Message)
set_target_properties(Connector PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/Connector.hpp")
install(TARGETS Connector)

add_library(ConnectorFactory "ConnectorFactory.cpp")
target_include_directories(ConnectorFactory PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(ConnectorFactory PUBLIC ConnectorInfos PUBLIC FlakedTuna PUBLIC Message)
set_target_properties(ConnectorFactory PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/ConnectorFactory.hpp")
install(TARGETS ConnectorFactory)

add_library(Infos "Infos.cpp")
target_link_libraries(Infos PUBLIC Exception PRIVATE magic_enum::magic_enum)
set_target_properties(Infos PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/Infos.hpp")
target_include_directories(Infos PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
install(TARGETS Infos)

add_library(Clients "Clients.cpp")
target_link_libraries(Clients PUBLIC ixwebsocket::ixwebsocket)
set_target_properties(Clients PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/Clients.hpp")
target_link_libraries(Clients PUBLIC Infos PUBLIC Exception)
target_include_directories(Clients PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
install(TARGETS Clients)



add_library(BoardInfos "BoardInfos.cpp")
target_include_directories(BoardInfos PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(BoardInfos PUBLIC Infos PUBLIC toml11::toml11)
set_target_properties(BoardInfos PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/BoardInfos.hpp")
install(TARGETS BoardInfos)

add_library(ConfigurationLoader "ConfigurationLoader.cpp")
target_include_directories(ConfigurationLoader PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(ConfigurationLoader PUBLIC ConnectorInfos PUBLIC BoardInfos)
set_target_properties(ConfigurationLoader PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/ConfigurationLoader.hpp")
install(TARGETS ConfigurationLoader)

add_library(WebSocketClient "WebSocketClient.cpp")
target_link_libraries(WebSocketClient PUBLIC Exception PUBLIC GeneralParameters PUBLIC ixwebsocket::ixwebsocket)
target_include_directories(WebSocketClient PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
set_target_properties(WebSocketClient PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/WebSocketClient.hpp")
install(TARGETS WebSocketClient)

add_library(Board "Module.cpp" "Board.cpp")
target_link_libraries(Board PUBLIC Message PUBLIC MessageHandlerClient PUBLIC WebSocketClient PUBLIC ConfigurationLoader PUBLIC ConnectorFactory PRIVATE Connector PUBLIC Identifier PUBLIC Interrupt)
set_target_properties(Board PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/Module.hpp" PUBLIC_HEADER "${YAODAQ_INCLUDE}/Board.hpp")
install(TARGETS Board)

add_library(Logger "Logger.cpp")
target_link_libraries(Logger PUBLIC Board)
set_target_properties(Logger PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/Logger.hpp")
install(TARGETS Logger)

add_library(WebSocketServer "WebSocketServer.cpp")
target_include_directories(WebSocketServer PRIVATE $<BUILD_INTERFACE:${YAODAQ_INCLUDE}>)
set_target_properties(WebSocketServer PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/WebSocketServer.hpp")
target_link_libraries(WebSocketServer PUBLIC Message PUBLIC Clients PUBLIC MessageHandlerServer PUBLIC Exception PUBLIC Identifier PUBLIC Interrupt PUBLIC GeneralParameters)

add_library(FileWritter "FileWritter.cpp")
target_link_libraries(FileWritter PUBLIC File PUBLIC Board)
set_target_properties(FileWritter PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/FileWritter.hpp")
install(TARGETS FileWritter)

if(BUILD_CONFIGURATOR)
  # include(soci) add_library(Configurator_ "${YAODAQ_SRC}/Configurator.cpp") add_dependencies(Configurator_ soci WebsocketServer Board) target_include_directories( Configurator_ PRIVATE $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC
    # ${DATABASES_INCLUDE_DIR}) set_target_properties( Configurator_ PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/Configurator.hpp") target_link_libraries( Configurator_ PUBLIC Board PUBLIC WebsocketServer PUBLIC Soci)
endif()
