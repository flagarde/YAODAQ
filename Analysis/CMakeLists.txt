add_executable(Analysis Analysis.cpp)
add_dependencies(Analysis Channel Event)
link_libraries(Analysis Event Channel Event ${ROOT_LIBRARIES} Event Channel Event)

target_link_libraries(Analysis Event Channel Event ${ROOT_LIBRARIES} Event Channel Event)

install(TARGETS Analysis RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})



