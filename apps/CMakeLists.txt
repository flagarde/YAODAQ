
add_executable(Analysis Analysis.cpp)
target_link_libraries(Analysis ${ROOT_LIBRARIES} Event_static Channel_static)
target_include_directories(Analysis PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/" "${CMAKE_SOURCE_DIR}/source/include/")
install(TARGETS Analysis RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})

if(NOT ${ANALYSER}) 
    add_executable(WaveDump WaveDump.cpp)
    find_package(ZLIB)
    target_include_directories(WaveDump PRIVATE ${ZLIB_INCLUDE_DIRS})
    add_dependencies(WaveDump lib1 lib2 web elogpp IXWebSocket CAENlibs)
    target_include_directories(WaveDump PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/" PUBLIC "${INCLUDE_OUTPUT_DIRECTORY}/ixwebsocket" "${INCLUDE_OUTPUT_DIRECTORY}/CAEN" "${INCLUDE_OUTPUT_DIRECTORY}/elogpp")
    target_link_libraries(WaveDump ${ROOT_LIBRARIES} CAENDigitizer CAENComm lib1 lib2 web  elogcpp jsoncpp m crypto Channel_static Event_static  ${ZLIB_LIBRARIES} mbedcrypto ixwebsocket mbedtls mbedx509)
    install(TARGETS WaveDump RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
    
    add_executable(WebsocketServer WebsocketServer.cpp)
    add_dependencies(WebsocketServer IXWebSocket websocket fmt CLI11)
    target_include_directories(WebsocketServer  PUBLIC "${INCLUDE_OUTPUT_DIRECTORY}/ixwebsocket" PUBLIC "${INCLUDE_OUTPUT_DIRECTORY}/" PRIVATE "${CMAKE_SOURCE_DIR}/source/include"  )
    target_link_libraries(WebsocketServer websocket mbedcrypto ixwebsocket mbedtls mbedx509 ${ZLIB_LIBRARIES} ${FMT_LIB} )
    install(TARGETS WebsocketServer RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
    
    add_executable(DumpModule DumpModule.cpp)
    add_dependencies(DumpModule IXWebSocket websocket fmt message)
    target_include_directories(DumpModule  PUBLIC "${INCLUDE_OUTPUT_DIRECTORY}/ixwebsocket" PUBLIC "${INCLUDE_OUTPUT_DIRECTORY}/" PRIVATE "${CMAKE_SOURCE_DIR}/source/include" PUBLIC "${INCLUDE_OUTPUT_DIRECTORY}/json")
    target_link_libraries(DumpModule websocket mbedcrypto ixwebsocket mbedtls mbedx509 ${ZLIB_LIBRARIES} ${FMT_LIB} jsoncpp message)
    install(TARGETS DumpModule RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
endif()

