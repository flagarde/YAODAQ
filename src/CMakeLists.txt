set(YAODAQ_INCLUDE "${CMAKE_SOURCE_DIR}/include")
set(YAODAQ_SRC "${CMAKE_CURRENT_SOURCE_DIR}")


# Include zlib-ng and openssl even if IXWebSocket include them too.
include(Zlib-ng)
include(OpenSSL)
include(IXWebSocket)
include(FlakedTuna)
# fmt and spdlog have difficulties to work together

include(Spdlog)
include(Fmt)
include(MagicEnum)
include(Jsoncpp)
include(toml11)
include(elogpp)


configure_file("ProgramInfos.hpp.in" "${CMAKE_BINARY_DIR}/ProgramInfos.hpp" @ONLY)
add_library(ProgramInfos INTERFACE)
set_target_properties(ProgramInfos PROPERTIES PUBLIC_HEADER "${CMAKE_BINARY_DIR}/ProgramInfos.hpp")
target_link_libraries(ProgramInfos INTERFACE fmt::fmt)
target_include_directories(ProgramInfos INTERFACE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}> INTERFACE $<INSTALL_INTERFACE:${CMAKE_INSTALL_FULL_INCLUDEDIR}>)
install(TARGETS ProgramInfos RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR} ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR} PUBLIC_HEADER DESTINATION  ${CMAKE_INSTALL_INCLUDEDIR})


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
target_include_directories(Message PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)
set_target_properties(Message PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/Message.hpp")
target_link_libraries(Message PUBLIC Exception PUBLIC jsoncpp_static PUBLIC magic_enum::magic_enum)
install(TARGETS Message RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR} ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR} PUBLIC_HEADER DESTINATION include)

add_library(Key "${YAODAQ_SRC}/Key.cpp")
target_include_directories(Key PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)
set_target_properties(Key PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/Key.hpp")
target_link_libraries(Key PRIVATE SourceLocation PRIVATE fmt::fmt)
install(TARGETS Key RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR} ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR} PUBLIC_HEADER DESTINATION include)

add_library(Interrupt "${YAODAQ_SRC}/Interrupt.cpp")
target_include_directories(Interrupt PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)
set_target_properties(Interrupt PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/Interrupt.hpp")
target_link_libraries(Interrupt PUBLIC spdlog::spdlog PRIVATE SourceLocation)
install(TARGETS Interrupt RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR} ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR} PUBLIC_HEADER DESTINATION include)

add_library(File "${YAODAQ_SRC}/File.cpp")
target_include_directories(File PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)
set_target_properties(File PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/File.hpp")
target_link_libraries(File PUBLIC Message)
install(TARGETS File RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR} ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR} PUBLIC_HEADER DESTINATION include)

add_library(Controller "${YAODAQ_SRC}/Controller.cpp")
target_link_libraries(Controller PUBLIC WebSocketClient PUBLIC Message PUBLIC Exception PUBLIC toml11::toml11 PUBLIC spdlog::spdlog )
target_include_directories(Controller PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)
set_target_properties(Controller PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/Controller.hpp")
install(TARGETS Controller RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR} ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR} PUBLIC_HEADER DESTINATION include)

add_library(ConnectorInfos "${YAODAQ_SRC}/ConnectorInfos.cpp")
target_include_directories(ConnectorInfos PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)
set_target_properties(ConnectorInfos PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/ConnectorInfos.hpp")
target_link_libraries(ConnectorInfos PUBLIC Exception PUBLIC toml11::toml11)
install(TARGETS ConnectorInfos RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR} ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR} PUBLIC_HEADER DESTINATION include)

add_library(Connector "${YAODAQ_SRC}/Connector.cpp")
target_include_directories(Connector PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)
target_link_libraries(Connector PUBLIC ConnectorInfos PUBLIC Message)
set_target_properties(Connector PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/Connector.hpp")
install(TARGETS Connector RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR} ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR} PUBLIC_HEADER DESTINATION include)

add_library(ConnectorFactory "${YAODAQ_SRC}/ConnectorFactory.cpp")
target_include_directories(ConnectorFactory PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)
target_link_libraries(ConnectorFactory PUBLIC ConnectorInfos PUBLIC FlakedTuna)
set_target_properties(ConnectorFactory PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/ConnectorFactory.hpp")
install(TARGETS ConnectorFactory RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR} ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR} PUBLIC_HEADER DESTINATION include)

add_library(Infos "${YAODAQ_SRC}/Infos.cpp")
target_link_libraries(Infos PUBLIC Exception PRIVATE magic_enum::magic_enum)
set_target_properties(Infos PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/Infos.hpp")
target_include_directories(Infos PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)
install(TARGETS Infos RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR} ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR} PUBLIC_HEADER DESTINATION include)

add_library(BoardInfos "${YAODAQ_SRC}/BoardInfos.cpp")
target_include_directories(BoardInfos PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)
target_link_libraries(BoardInfos PUBLIC Infos PUBLIC toml11::toml11)
set_target_properties(BoardInfos PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/BoardInfos.hpp")
install(TARGETS BoardInfos RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR} ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR} PUBLIC_HEADER DESTINATION include)

add_library(ConfigurationLoader "${YAODAQ_SRC}/ConfigurationLoader.cpp")
target_include_directories(ConfigurationLoader PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)
target_link_libraries(ConfigurationLoader PUBLIC ConnectorInfos PUBLIC BoardInfos)
set_target_properties(ConfigurationLoader PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/ConfigurationLoader.hpp")
install(TARGETS ConfigurationLoader RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR} PUBLIC_HEADER DESTINATION include)

add_library(WebSocketClient "${YAODAQ_SRC}/WebsocketClient.cpp" "${YAODAQ_SRC}/GeneralParameters.cpp")
target_link_libraries(WebSocketClient PUBLIC IXWebSocket PUBLIC Exception)
target_include_directories(WebSocketClient PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)
set_target_properties(WebSocketClient PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/WebsocketClient.hpp;${YAODAQ_INCLUDE}/GeneralParameters.hpp")
install(TARGETS WebSocketClient RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR} ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR} PUBLIC_HEADER DESTINATION include)

add_library(Board "${YAODAQ_SRC}/Module.cpp" "${YAODAQ_SRC}/Board.cpp")
target_link_libraries(Board PUBLIC Message PUBLIC WebSocketClient PUBLIC ConfigurationLoader PUBLIC ConnectorFactory PRIVATE Connector PUBLIC spdlog::spdlog)
set_target_properties(Board PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/Module.hpp" PUBLIC_HEADER "${YAODAQ_INCLUDE}/Board.hpp")
install(TARGETS Board RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR} ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR} PUBLIC_HEADER DESTINATION include)

add_library(Logger "${YAODAQ_SRC}/Logger.cpp")
target_link_libraries(Logger PUBLIC Message PUBLIC WebSocketClient PUBLIC Exception PUBLIC spdlog::spdlog)
set_target_properties(Logger PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/Logger.hpp")
install(TARGETS Logger RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR} ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR} PUBLIC_HEADER DESTINATION include)

if(BUILD_WEBSOCKETSERVER)
  add_library(WebsocketServer "${YAODAQ_SRC}/WebsocketServer.cpp")
  target_include_directories(WebsocketServer PRIVATE $<BUILD_INTERFACE:${YAODAQ_INCLUDE}>)
  set_target_properties(WebsocketServer PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/WebsocketServer.hpp")
  target_link_libraries(WebsocketServer PUBLIC spdlog::spdlog PUBLIC Message PUBLIC Infos PUBLIC IXWebSocket PUBLIC Exception)

  add_library(FileWritter "${YAODAQ_SRC}/FileWritter.cpp")
  target_link_libraries(FileWritter PUBLIC File PUBLIC Board)
  set_target_properties(FileWritter PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/FileWritter.hpp")
  install(TARGETS FileWritter RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR} ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR} PUBLIC_HEADER DESTINATION include)

  if(BUILD_CONFIGURATOR)
    # include(soci) add_library(Configurator_ "${YAODAQ_SRC}/Configurator.cpp") add_dependencies(Configurator_ soci WebsocketServer Board) target_include_directories( Configurator_ PRIVATE $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC
    # ${DATABASES_INCLUDE_DIR}) set_target_properties( Configurator_ PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDE}/Configurator.hpp") target_link_libraries( Configurator_ PUBLIC Board PUBLIC WebsocketServer PUBLIC Soci)
  endif()

endif()
