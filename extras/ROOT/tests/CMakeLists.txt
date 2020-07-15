set(TESTFILES  RootFile.cpp)

set(TEST_RUNNER_PARAMS "") # Any arguemnts to feed the test runner (change as needed).

# ------------------------------------------------------------------------------
# -- Make Tests (no change needed).
# ------------------------------------------------------------------------------
# --
add_executable(RootTests ${TESTFILES})
add_dependencies(RootTests DoctestMain RootFile)

set_target_properties(RootTests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX})
# target_set_warnings(${TEST_MAIN} ENABLE ALL AS_ERROR ALL DISABLE Annoying) # Set warnings (if needed).
target_include_directories(RootTests PUBLIC "${INCLUDE_OUTPUT_DIR}")

target_link_libraries(RootTests PUBLIC RootFile PUBLIC DoctestMain)
doctest_discover_tests(RootTests COMMAND RootTests ${TEST_RUNNER_PARAMS})

