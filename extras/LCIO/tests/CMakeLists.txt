set(TESTFILES LCIOFile.cpp)

set(TEST_RUNNER_PARAMS "") # Any arguemnts to feed the test runner (change as needed).

# ------------------------------------------------------------------------------
# -- Make Tests (no change needed).
# ------------------------------------------------------------------------------
# --
add_executable(LCIOTests ${TESTFILES})
add_dependencies(LCIOTests doctestMain LCIOFile)

set_target_properties(LCIOTests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX})
# target_set_warnings(${TEST_MAIN} ENABLE ALL AS_ERROR ALL DISABLE Annoying) # Set warnings (if needed).
target_include_directories(LCIOTests PUBLIC "${INCLUDE_OUTPUT_DIR}")

target_link_libraries(LCIOTests PUBLIC LCIOFile PUBLIC doctestMain)
doctest_discover_tests(LCIOTests COMMAND LCIOTests ${TEST_RUNNER_PARAMS})
