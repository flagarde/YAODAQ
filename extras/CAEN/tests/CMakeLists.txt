# List all files containing tests. (Change as needed)
set(TESTFILES CAENErrors.cpp)

set(TEST_RUNNER_PARAMS "") # Any arguemnts to feed the test runner (change as needed).

# ------------------------------------------------------------------------------
# -- Make Tests (no change needed).
# ------------------------------------------------------------------------------
# --
add_executable(Tests ${TESTFILES})
add_dependencies(Tests doctestMain CAENExceptions)
set_target_properties(Tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX})
# target_set_warnings(${TEST_MAIN} ENABLE ALL AS_ERROR ALL DISABLE Annoying) # Set warnings (if needed).
target_include_directories(Tests PUBLIC "${INCLUDE_OUTPUT_DIR}")

target_link_libraries(Tests PUBLIC CAENExceptions PUBLIC doctestMain)

doctest_discover_tests(Tests COMMAND Tests ${TEST_RUNNER_PARAMS})
