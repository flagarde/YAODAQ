add_executable(Analysis Analysis.cpp)
add_dependencies(Analysis Channel)
link_libraries(Analysis ${ROOT_LIBRARIES} Channel)

target_link_libraries(Analysis ${ROOT_LIBRARIES} Channel)

install(TARGETS Analysis RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})



