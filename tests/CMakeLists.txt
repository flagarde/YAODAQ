include(CodeCoverage)

add_executable(ExceptionsTests Exceptions.test.cpp)
set_target_properties(ExceptionsTests PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}/tests")
target_link_libraries(ExceptionsTests PRIVATE Exception PRIVATE doctest_with_main)
doctest_discover_tests(ExceptionsTests COMMAND Exceptions ADD_LABELS "Test Exceptions")

add_executable(GeneralParametersTest GeneralParameters.test.cpp)
set_target_properties(GeneralParametersTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}/tests")
target_link_libraries(GeneralParametersTest PRIVATE GeneralParameters PRIVATE doctest_with_main)
doctest_discover_tests(GeneralParametersTest COMMAND GeneralParametersTest ADD_LABELS "GeneralParametersTest" WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")


#add_executable(MessagesTests Messages.test.cpp)
#set_target_properties(MessagesTests PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}/tests")
#target_link_libraries(MessagesTests PRIVATE Message PRIVATE doctest_with_main)
#doctest_discover_tests(MessagesTests COMMAND MessagesTests ADD_LABELS "Test Messages")

#add_executable(MessageHandlerTests MessageHandler.test.cpp)
#set_target_properties(MessageHandlerTests PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}/tests")
#target_link_libraries(MessageHandlerTests PRIVATE MessageHandlerClient PRIVATE doctest_with_main)
#doctest_discover_tests(MessageHandlerTests COMMAND MessageHandlerTests ADD_LABELS "Test MessageHandler")

add_executable(UtilitiesTests Utilities.test.cpp)
set_target_properties(UtilitiesTests PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}/tests")
target_link_libraries(UtilitiesTests PRIVATE Utilities PRIVATE doctest_with_main)
doctest_discover_tests(UtilitiesTests COMMAND UtilitiesTests ADD_LABELS "Test Utilities")
