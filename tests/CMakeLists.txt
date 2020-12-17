include(CodeCoverage)
set(TESTFILES FileWritter.cpp)

set(TEST_RUNNER_PARAMS "") # Any arguemnts to feed the test runner (change as needed).

# ------------------------------------------------------------------------------
# -- Make Tests (no change needed).
# ------------------------------------------------------------------------------
# --
#add_executable(TestFileWritter ${TESTFILES})
#add_dependencies(TestFileWritter doctestMain)

#set_target_properties(TestFileWritter PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX})
# target_set_warnings(${TEST_MAIN} ENABLE ALL AS_ERROR ALL DISABLE Annoying) # Set warnings (if needed).
#target_include_directories(TestFileWritter PUBLIC "${INCLUDE_OUTPUT_DIR}")

#target_link_libraries(TestFileWritter PUBLIC FileWritter PUBLIC doctestMain)
#doctest_discover_tests(TestFileWritter COMMAND FileWritter ${TEST_RUNNER_PARAMS})
# doctest_discover_tests(Tests) add_test(NAME Tests COMMAND Tests ${TEST_RUNNER_PARAMS} )


add_executable(Exceptions Exceptions.cpp)
add_dependencies(Exceptions doctest_with_main Exception)
set_target_properties(Exceptions PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX})
# target_set_warnings(${TEST_MAIN} ENABLE ALL AS_ERROR ALL DISABLE Annoying) # Set warnings (if needed).
target_include_directories(Exceptions PUBLIC "${INCLUDE_OUTPUT_DIR}")
target_link_libraries(Exceptions PUBLIC Exception PUBLIC doctest_with_main)
doctest_discover_tests(Exceptions COMMAND Exceptions ${TEST_RUNNER_PARAMS})
