configure_file(
  "include/ProgramInfos.hpp.in"
  "${GENERATED_HEADER_PATH}/ProgramInfos.hpp"
)

ROOT_GENERATE_DICTIONARY(G__Channel ${CMAKE_CURRENT_SOURCE_DIR}/include/Channel.hpp LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/include/ChannelLinkDef.hpp)
add_library(Channel OBJECT src/Channel.cpp G__Channel.cxx)
target_link_libraries(Channel INTERFACE ${ROOT_LIBRARIES})
target_include_directories(Channel PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
add_library(Channel_shared SHARED $<TARGET_OBJECTS:Channel>)
add_library(Channel_static STATIC $<TARGET_OBJECTS:Channel>)
add_custom_command(TARGET Channel_shared PRE_BUILD  COMMAND ${CMAKE_COMMAND} -E copy
                "${CMAKE_CURRENT_BINARY_DIR}/libChannel_rdict.pcm" "${RUNTIME_OUTPUT_DIRECTORY}/libChannel_rdict.pcm"
                )


ROOT_GENERATE_DICTIONARY(G__Event ${CMAKE_CURRENT_SOURCE_DIR}/include/Event.hpp LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/include/EventLinkDef.hpp)
add_library(Event OBJECT src/Event.cpp G__Event.cxx)
target_include_directories(Event PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_link_libraries(Event INTERFACE ${ROOT_LIBRARIES})
add_library(Event_shared SHARED $<TARGET_OBJECTS:Event>)
add_library(Event_static STATIC $<TARGET_OBJECTS:Event>)
add_custom_command(TARGET Event_shared PRE_BUILD  COMMAND ${CMAKE_COMMAND} -E copy
                "${CMAKE_CURRENT_BINARY_DIR}/libEvent_rdict.pcm" "${RUNTIME_OUTPUT_DIRECTORY}/libEvent_rdict.pcm"
                )

if(NOT ${ANALYSER}) 
    
    if(CAEN_HARDWARE)
        add_subdirectory(CAEN)
    endif()
    
    add_library(websocket src/WebsocketServer.cpp src/WebsocketClient.cpp src/ConnectionState.cpp)
    add_dependencies(websocket IXWebSocket)
    target_include_directories(websocket PUBLIC "${INCLUDE_OUTPUT_DIRECTORY}/ixwebsocket" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/")
    
    
    add_library(lib1 STATIC src/flash.cpp src/WDconfig.cpp src/X742CorrectionRoutines.cpp src/spi.cpp src/Digitizer.cpp)
    add_dependencies(lib1 CAENlibs)
    target_include_directories(lib1 PUBLIC "${INCLUDE_OUTPUT_DIRECTORY}/CAEN" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
    
    add_library(lib2 STATIC src/FileManager.cpp )
    add_dependencies(lib2 CAENlibs)
    target_include_directories(lib2 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/" PUBLIC "${INCLUDE_OUTPUT_DIRECTORY}/CAEN")
    
    add_library(web STATIC src/WebServer.cpp src/Plotter.cpp)
    add_dependencies(web elogpp IXWebSocket CAENlibs)
    target_include_directories(web PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/" PUBLIC "${INCLUDE_OUTPUT_DIRECTORY}/ixwebsocket" "${INCLUDE_OUTPUT_DIRECTORY}/CAEN")
endif()



