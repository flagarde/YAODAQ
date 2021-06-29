include(CodeCoverage)

add_executable(Exceptions Exceptions.cpp)
set_target_properties(Exceptions PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX})
target_link_libraries(Exceptions PRIVATE Exception PRIVATE doctest_with_main)
doctest_discover_tests(Exceptions COMMAND Exceptions ADD_LABELS "Exceptions")


#add_executable(MessagesTests Messages.test.cpp)
#set_target_properties(MessagesTests PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}/tests")
#target_link_libraries(MessagesTests PRIVATE Message PRIVATE doctest_with_main)
#doctest_discover_tests(MessagesTests COMMAND MessagesTests ADD_LABELS "Test Messages")

add_executable(MessageHandlerTests MessageHandler.test.cpp)
set_target_properties(MessageHandlerTests PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}/tests")
target_link_libraries(MessageHandlerTests PRIVATE MessageHandlerClient #PRIVATE doctest_with_main
)
doctest_discover_tests(MessageHandlerTests COMMAND MessageHandlerTests ADD_LABELS "Test MessageHandler")

add_executable(UtilitiesTests Utilities.test.cpp)
set_target_properties(UtilitiesTests PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}/tests")
target_link_libraries(UtilitiesTests PRIVATE Utilities PRIVATE doctest_with_main)
doctest_discover_tests(UtilitiesTests COMMAND UtilitiesTests ADD_LABELS "Test Utilities")

add_executable(MessageTest2 Message2.test.cpp)
set_target_properties(MessageTest2 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}/tests")
target_link_libraries(MessageTest2 PRIVATE Message2 PRIVATE doctest_with_main)
doctest_discover_tests(MessageTest2 COMMAND MessageTest2 ADD_LABELS "Test Messages")
