find_package(Git REQUIRED)

set(YAODAQ_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(YAODAQ_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src")

configure_file("${YAODAQ_SRC}/ProgramInfos.hpp.in" "${INCLUDE_OUTPUT_DIRECTORY}/ProgramInfos.hpp" @ONLY)

find_package(Threads)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

include(IXWebSocket)
add_library(WebSocket INTERFACE)
add_dependencies(WebSocket IXWebSocket)
if(NOT WIN32)
  find_package(ZLIB)
  target_link_libraries(WebSocket INTERFACE ZLIB::ZLIB INTERFACE mbedcrypto INTERFACE ixwebsocket INTERFACE mbedtls INTERFACE mbedx509)
  target_include_directories(WebSocket INTERFACE "${INCLUDE_OUTPUT_DIRECTORY}/ixwebsocket")
else()
  target_link_libraries(WebSocket INTERFACE mbedcrypto INTERFACE ixwebsocket INTERFACE mbedtls INTERFACE mbedx509) 
  target_include_directories(WebSocket INTERFACE "${INCLUDE_OUTPUT_DIRECTORY}/ixwebsocket")
endif()

include(CLI11)
include(spdlog)
add_library(Util INTERFACE)
add_dependencies(Util CLI11 spdlog)
target_link_libraries(Util INTERFACE spdlog)
target_include_directories(Util INTERFACE "${INCLUDE_OUTPUT_DIRECTORY}" INTERFACE "${INCLUDE_OUTPUT_DIRECTORY}/spdlog")

include(elogpp)
include(soci)
include(toml11)

if(BUILD_ANALYSIS)
  ROOT_GENERATE_DICTIONARY(G__Channel ${YAODAQ_INCLUDE}/Channel.hpp LINKDEF ${YAODAQ_INCLUDE}/ChannelLinkDef.hpp)
  add_library(Channel OBJECT src/Channel.cpp G__Channel.cxx)
  target_include_directories(Channel PRIVATE ${YAODAQ_INCLUDE})
  add_library(Channel_shared SHARED $<TARGET_OBJECTS:Channel>)
  target_include_directories(Channel_shared PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIRECTORY}>)
  target_link_libraries(Channel_shared INTERFACE ${ROOT_LIBRARIES} INTERFACE Threads::Threads)
  add_library(Channel_static STATIC $<TARGET_OBJECTS:Channel>)
  target_link_libraries(Channel_static INTERFACE ${ROOT_LIBRARIES} INTERFACE Threads::Threads)
  target_include_directories(Channel_static PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIRECTORY}>)
  add_custom_target(CopyChannel DEPENDS ALL COMMAND ${CMAKE_COMMAND} -E copy libChannel_rdict.pcm "${RUNTIME_OUTPUT_DIRECTORY}/libChannel_rdict.pcm")
  set_target_properties(Channel PROPERTIES PUBLIC_HEADER ${YAODAQ_INCLUDE}/Channel.hpp)
  install(TARGETS Channel RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION include)


  ROOT_GENERATE_DICTIONARY(G__Event ${YAODAQ_INCLUDE}/Event.hpp LINKDEF ${YAODAQ_INCLUDE}/EventLinkDef.hpp)
  add_library(Event OBJECT src/Event.cpp G__Event.cxx)
  target_include_directories(Event PRIVATE ${YAODAQ_INCLUDE})
  target_link_libraries(Event INTERFACE ${ROOT_LIBRARIES})
  add_library(Event_shared SHARED $<TARGET_OBJECTS:Event>)
  target_include_directories(Event_shared PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIRECTORY}>)
  target_link_libraries(Event_shared INTERFACE ${ROOT_LIBRARIES} INTERFACE Threads::Threads)
  add_library(Event_static STATIC $<TARGET_OBJECTS:Event>)
  target_include_directories(Event_static PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIRECTORY}>)
  target_link_libraries(Event_static INTERFACE ${ROOT_LIBRARIES} INTERFACE Threads::Threads)
  add_custom_target(CopyEvent DEPENDS ALL COMMAND ${CMAKE_COMMAND} -E copy libEvent_rdict.pcm "${RUNTIME_OUTPUT_DIRECTORY}/libEvent_rdict.pcm")
  set_target_properties(Event PROPERTIES PUBLIC_HEADER ${YAODAQ_INCLUDE}/Event.hpp)
  install(TARGETS Event RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION include)
endif()

add_library(Modules INTERFACE)

if(NOT ${ANALYSER}) 
    
    if(CAEN_HARDWARE)
      add_subdirectory(CAEN)
      target_link_libraries(Modules INTERFACE CAENConnectors)
      target_include_directories(Modules INTERFACE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/CAEN/include> INTERFACE $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIRECTORY}/CAEN>)
    endif()
    
    add_library(Error src/Error.cpp include/Error.hpp)
    target_include_directories(Error PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIRECTORY}> )
    install(TARGETS Error RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION include)

    add_library(ConnectorInfos src/ConnectorInfos.cpp include/ConnectorInfos.hpp)
    add_dependencies(ConnectorInfos toml11)
    target_include_directories(ConnectorInfos PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIRECTORY}> PUBLIC "${INCLUDE_OUTPUT_DIRECTORY}/toml" PUBLIC "${INCLUDE_OUTPUT_DIRECTORY}/toml/toml") #FIXME TOML/TOML DUE TO CMAKEFILE
    install(TARGETS ConnectorInfos RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION include)
    
    add_library(Connector src/Connector.cpp include/Connector.hpp)
    add_dependencies(Connector ConnectorInfos)
    target_include_directories(Connector PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIRECTORY}>)
    install(TARGETS Connector RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION include)
    target_link_libraries(Connector PUBLIC ConnectorInfos)
    
    add_library(ConnectorFactory src/ConnectorFactory.cpp include/ConnectorFactory.hpp)
    add_dependencies(ConnectorFactory ConnectorInfos Modules)
    target_include_directories(ConnectorFactory PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIRECTORY}>)
    target_link_libraries(ConnectorFactory PUBLIC ConnectorInfos PUBLIC Modules)
    install(TARGETS ConnectorFactory RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION include)
    
    
    add_library(Configuration src/Configuration.cpp src/BoardInfos.cpp)
    add_dependencies(Configuration ConnectorInfos)
    target_include_directories(Configuration PUBLIC $<BUILD_INTERFACE:${YAODAQ_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIRECTORY}>)
    install(TARGETS Configuration RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION include)
    target_link_libraries(Configuration PUBLIC ConnectorInfos)
    
    add_library(message src/Message.cpp src/Info.cpp src/Status.cpp src/Log.cpp)
    add_dependencies(message elogpp)
    target_include_directories(message PUBLIC "${INCLUDE_OUTPUT_DIRECTORY}/json" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/")
    target_link_libraries(message PUBLIC jsoncpp)
    
    add_library(websocket src/WebsocketServer.cpp src/WebsocketClient.cpp src/ConnectionState.cpp src/Module.cpp  src/Connector.cpp  src/Board.cpp )
    add_dependencies(websocket Util WebSocket message Configuration ConnectorFactory)
    target_include_directories(websocket  PUBLIC "${INCLUDE_OUTPUT_DIRECTORY}/toml"  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/")
    target_link_libraries(websocket PUBLIC message PUBLIC WebSocket PUBLIC Util PUBLIC Configuration PUBLIC ConnectorFactory)
   
    
    
    
  if(${CONFIGURATOR})
    add_library(configurator src/Configurator.cpp)
    add_dependencies(configurator soci websocket toml11)
    target_include_directories(configurator PRIVATE "${INCLUDE_OUTPUT_DIRECTORY}/" PUBLIC ${DATABASES_INCLUDE_DIR}  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/")
    target_link_libraries(configurator PUBLIC websocket ${DATABASES_LIBRARIES} PUBLIC soci_core PUBLIC soci_empty)
    if(${DB2_FOUND})
      message(STATUS "Linking soci_db2")
      target_link_libraries(configurator PUBLIC soci_db2)
    endif()
    if(${FIREBIRD_FOUND})
      message(STATUS "Linking soci_firebird")
      target_link_libraries(configurator PUBLIC soci_firebird)
    endif()
    if(${MYSQL_FOUND})
      message(STATUS "Linking soci_mysql")
      target_link_libraries(configurator PUBLIC soci_mysql)
    endif()
    if(${ODBC_FOUND})
      message(STATUS "Linking soci_odbc")
      target_link_libraries(configurator PUBLIC soci_odbc)
    endif()
    if(${ORACLE_FOUND})
      message(STATUS "Linking soci_oracle")
      target_link_libraries(configurator PUBLIC soci_oracle)
    endif()
    if(${POSTGRESQL_FOUND})
      message(STATUS "Linking soci_postgresql")
      target_link_libraries(configurator PUBLIC soci_postgresql)
    endif()
    if(${SQLITE3_FOUND})
      message(STATUS "Linking soci_sqlite3")
      target_link_libraries(configurator PUBLIC soci_sqlite3)
    endif()
  endif()

    
    
    
   
    
    
    add_library(lib1 STATIC src/flash.cpp src/WDconfig.cpp src/X742CorrectionRoutines.cpp src/spi.cpp src/Digitizer.cpp)
    add_dependencies(lib1 CAENlibs)
    target_include_directories(lib1 PUBLIC "${INCLUDE_OUTPUT_DIRECTORY}/CAEN" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/" PRIVATE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/CAEN>)
    
    add_library(lib2 STATIC src/FileManager.cpp )
    add_dependencies(lib2 CAENlibs)
    target_include_directories(lib2 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/" PUBLIC "${INCLUDE_OUTPUT_DIRECTORY}/CAEN" PRIVATE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/CAEN>)
    target_link_libraries(lib2 PUBLIC ZLIB::ZLIB)
    
    add_library(web STATIC src/WebServer.cpp src/Plotter.cpp)
    add_dependencies(web elogpp IXWebSocket CAENlibs)
    target_include_directories(web PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/" PUBLIC "${INCLUDE_OUTPUT_DIRECTORY}/ixwebsocket" PRIVATE "${INCLUDE_OUTPUT_DIRECTORY}/CAEN" PRIVATE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/CAEN>)
endif()



