set(YAODAQ_INCLUDE "${CMAKE_SOURCE_DIR}/include")
set(YAODAQ_SRC "${CMAKE_CURRENT_SOURCE_DIR}")

configure_file("${YAODAQ_SRC}/ProgramInfos.hpp.in" "${INCLUDE_OUTPUT_DIR}/ProgramInfos.hpp" @ONLY)

# Include zlib-ng and openssl even if IXWebSocket include them too.
include(zlib-ng)
include(openssl)
include(IXWebSocket)
include(FlakedTuna)
# fmt and spdlog have difficulties to work together
include(fmt)
include(spdlog)

include(magic_enum)
include(jsoncpp)
include(toml11)
include(elogpp)
add_library(Util INTERFACE)
add_dependencies(Util spdlog::spdlog jsoncpp::jsoncpp toml11 magic_enum elogpp::elogpp OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(Util INTERFACE spdlog::spdlog INTERFACE jsoncpp::jsoncpp INTERFACE elogpp::elogpp INTERFACE OpenSSL::SSL INTERFACE OpenSSL::Crypto)
target_include_directories(Util INTERFACE "${INCLUDE_OUTPUT_DIR}" INTERFACE "${INCLUDE_OUTPUT_DIR}/toml")


add_library(SourceLocation "${YAODAQ_SRC}/SourceLocation.cpp")
target_include_directories(SourceLocation PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)
set_target_properties(SourceLocation PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/SourceLocation.hpp")
install(TARGETS SourceLocation RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR} ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR} PUBLIC_HEADER DESTINATION include)

add_library(Exception "${YAODAQ_SRC}/Exception.cpp")
target_include_directories(Exception PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)
set_target_properties(Exception PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/Exception.hpp")
target_link_libraries(Exception PUBLIC fmt::fmt PRIVATE SourceLocation)
install(TARGETS Exception RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR} ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR} PUBLIC_HEADER DESTINATION include)




add_library(Message "${YAODAQ_SRC}/Message.cpp")
add_dependencies(Message Exception Util)
target_include_directories(Message PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)
set_target_properties(Message PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/Message.hpp")
target_link_libraries(Message PUBLIC Exception PUBLIC Util)
install(TARGETS Message RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR} ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR} PUBLIC_HEADER DESTINATION include)



add_library(Key "${YAODAQ_SRC}/Key.cpp")
add_dependencies(Key Util SourceLocation)
target_include_directories(Key PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)
set_target_properties(Key PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/Key.hpp")
target_link_libraries(Key PUBLIC Util PRIVATE SourceLocation)
install(TARGETS Key RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR} ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR} PUBLIC_HEADER DESTINATION include)


add_library(Interrupt "${YAODAQ_SRC}/Interrupt.cpp")
add_dependencies(Interrupt Util SourceLocation)
target_include_directories(Interrupt PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)
set_target_properties(Interrupt PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/Interrupt.hpp")
target_link_libraries(Interrupt PUBLIC Util PRIVATE SourceLocation)
install(TARGETS Interrupt RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR} ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR} PUBLIC_HEADER DESTINATION include)


add_library(File "${YAODAQ_SRC}/File.cpp")
add_dependencies(File Util Message)
target_include_directories(File PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)
set_target_properties(File PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/File.hpp")
target_link_libraries(File PUBLIC Util PUBLIC Message)
install(TARGETS File RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR} ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR} PUBLIC_HEADER DESTINATION include)



add_library(Controller "${YAODAQ_SRC}/Controller.cpp")
add_dependencies(Controller WebSocketClient Message Exception)
target_link_libraries(Controller PUBLIC WebSocketClient PUBLIC Message PUBLIC Exception)
target_include_directories(Controller PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)
set_target_properties(Controller PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/Controller.hpp")
install(TARGETS Controller RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR} ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR} PUBLIC_HEADER DESTINATION include)

add_library(ConnectorInfos "${YAODAQ_SRC}/ConnectorInfos.cpp")
add_dependencies(ConnectorInfos Exception)
target_include_directories(ConnectorInfos PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}> PUBLIC "${INCLUDE_OUTPUT_DIR}/toml")
set_target_properties(ConnectorInfos PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/ConnectorInfos.hpp")
target_link_libraries(ConnectorInfos PUBLIC Exception)
install(TARGETS ConnectorInfos RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR} ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR} PUBLIC_HEADER DESTINATION include)

add_library(Connector "${YAODAQ_SRC}/Connector.cpp")
add_dependencies(Connector ConnectorInfos Message)
target_include_directories(Connector PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)
target_link_libraries(Connector PUBLIC ConnectorInfos PUBLIC Message)
set_target_properties(Connector PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/Connector.hpp")
install(TARGETS Connector RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR} ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR} PUBLIC_HEADER DESTINATION include)

add_library(ConnectorFactory "${YAODAQ_SRC}/ConnectorFactory.cpp")
add_dependencies(ConnectorFactory ConnectorInfos YAODAQ::FlakedTuna)
target_include_directories(ConnectorFactory PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)
target_link_libraries(ConnectorFactory PUBLIC ConnectorInfos  PUBLIC YAODAQ::FlakedTuna)
set_target_properties(ConnectorFactory PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/ConnectorFactory.hpp")
install(TARGETS ConnectorFactory RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR} ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR} PUBLIC_HEADER DESTINATION include)

add_library(Infos "${YAODAQ_SRC}/Infos.cpp")
add_dependencies(Infos Exception)
target_link_libraries(Infos PUBLIC Exception)
set_target_properties(Infos PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/Infos.hpp")
target_include_directories(Infos PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)
install(TARGETS Infos RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR} ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR} PUBLIC_HEADER DESTINATION include)

add_library(BoardInfos "${YAODAQ_SRC}/BoardInfos.cpp")
target_include_directories(BoardInfos PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}> PUBLIC "${INCLUDE_OUTPUT_DIR}/toml")
add_dependencies(BoardInfos toml11 Infos)
target_link_libraries(BoardInfos PUBLIC Infos)
set_target_properties(BoardInfos PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/BoardInfos.hpp")
install(TARGETS BoardInfos RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR} ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR} PUBLIC_HEADER DESTINATION include)

add_library(ConfigurationLoader "${YAODAQ_SRC}/ConfigurationLoader.cpp")
add_dependencies(ConfigurationLoader ConnectorInfos BoardInfos)
target_include_directories(ConfigurationLoader PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)
target_link_libraries(ConfigurationLoader PUBLIC ConnectorInfos PUBLIC BoardInfos PUBLIC Util)
set_target_properties(ConfigurationLoader PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/ConfigurationLoader.hpp")
install(TARGETS ConfigurationLoader RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR} PUBLIC_HEADER DESTINATION include)



add_library(WebSocketClient "${YAODAQ_SRC}/WebsocketClient.cpp" "${YAODAQ_SRC}/GeneralParameters.cpp")
add_dependencies(WebSocketClient IXWebSocket Util Exception)
target_link_libraries(WebSocketClient PUBLIC IXWebSocket PUBLIC Util PUBLIC Exception)
target_include_directories(WebSocketClient PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)
set_target_properties(WebSocketClient PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/WebsocketClient.hpp;${YAODAQ_INCLUDE}/GeneralParameters.hpp")
install(TARGETS WebSocketClient RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR} ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR} PUBLIC_HEADER DESTINATION include)

add_library(Board "${YAODAQ_SRC}/Module.cpp" "${YAODAQ_SRC}/Board.cpp")
add_dependencies(Board Message ConfigurationLoader ConnectorFactory WebSocketClient Connector)
target_link_libraries(Board PUBLIC Message PUBLIC WebSocketClient PUBLIC ConfigurationLoader PUBLIC ConnectorFactory PRIVATE Connector)
set_target_properties(Board PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/Module.hpp" PUBLIC_HEADER "${YAODAQ_INCLUDE}/Board.hpp")
install(TARGETS Board RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR} ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR} PUBLIC_HEADER DESTINATION include)

add_library(Logger "${YAODAQ_SRC}/Logger.cpp")
add_dependencies(Logger Message WebSocketClient Exception Util)
target_link_libraries(Logger PUBLIC Message PUBLIC WebSocketClient PUBLIC Exception PUBLIC Util)
set_target_properties(Logger PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/Logger.hpp")
install(TARGETS Logger RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR} ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR} PUBLIC_HEADER DESTINATION include)

if(BUILD_WEBSOCKETSERVER)
  add_library(WebsocketServer "${YAODAQ_SRC}/WebsocketServer.cpp")
  add_dependencies(WebsocketServer Util Message Infos IXWebSocket Exception)
  target_include_directories(WebsocketServer PRIVATE $<BUILD_INTERFACE:${YAODAQ_INCLUDE}>)
  set_target_properties(WebsocketServer PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/WebsocketServer.hpp")
  target_link_libraries(WebsocketServer PUBLIC Util PUBLIC Message PUBLIC Infos PUBLIC IXWebSocket PUBLIC Exception)

  add_library(FileWritter "${YAODAQ_SRC}/FileWritter.cpp")
  add_dependencies(FileWritter File Board)
  target_link_libraries(FileWritter PUBLIC File PUBLIC Board)
  set_target_properties(FileWritter PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/FileWritter.hpp")
  install(TARGETS FileWritter RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR} ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR} PUBLIC_HEADER DESTINATION include)

  if(BUILD_CONFIGURATOR)
    # include(soci) add_library(Configurator_ "${YAODAQ_SRC}/Configurator.cpp") add_dependencies(Configurator_ soci WebsocketServer Board) target_include_directories( Configurator_ PRIVATE $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC
    # ${DATABASES_INCLUDE_DIR}) set_target_properties( Configurator_ PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/Configurator.hpp") target_link_libraries( Configurator_ PUBLIC Board PUBLIC WebsocketServer PUBLIC Soci)
  endif()

endif()
