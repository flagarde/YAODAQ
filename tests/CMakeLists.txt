include(doctest)
include(CodeCoverage)

if(ENABLE_DOCTESTS)
  add_definitions(-DENABLE_DOCTEST_IN_LIBRARY)
endif()

# Expose required variable (DOCTEST_INCLUDE_DIR) to parent scope
externalproject_get_property(doctest source_dir)

set(DOCTEST_INCLUDE_DIR ${source_dir}/doctest CACHE INTERNAL "Path to include folder for doctest")
include_directories(doctest INTERFACE ${DOCTEST_INCLUDE_DIR})

# List all files containing tests. (Change as needed)
set(TESTFILES Main.cpp CAENErrors.cpp)

set(TEST_RUNNER_PARAMS "") # Any arguemnts to feed the test runner (change as needed).

# ------------------------------------------------------------------------------
# -- Make Tests (no change needed).
# ------------------------------------------------------------------------------
# --
add_executable(Tests ${TESTFILES})
add_dependencies(Tests doctest)

set_target_properties(Tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX})
# target_set_warnings(${TEST_MAIN} ENABLE ALL AS_ERROR ALL DISABLE Annoying) # Set warnings (if needed).
target_include_directories(Tests PUBLIC "${INCLUDE_OUTPUT_DIR}")

target_link_libraries(Tests PUBLIC CAENExceptions)
add_dependencies(Tests CAENExceptions)

# doctest_discover_tests(Tests) add_test(NAME Tests COMMAND Tests ${TEST_RUNNER_PARAMS} )
