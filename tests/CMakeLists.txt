# List all files containing tests. (Change as needed)
set(TESTFILES        # All .cpp files in tests/
    Main.cpp
    CAENErrors.cpp
)



set(TEST_MAIN Tests)   # Default name for test executable (change if you wish).
set(TEST_RUNNER_PARAMS "")  # Any arguemnts to feed the test runner (change as needed).

# --------------------------------------------------------------------------------
#                         Make Tests (no change needed).
# --------------------------------------------------------------------------------
add_executable(${TEST_MAIN} ${TESTFILES})
add_dependencies(${TEST_MAIN} doctest)

set_target_properties(${TEST_MAIN}  PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
#target_set_warnings(${TEST_MAIN} ENABLE ALL AS_ERROR ALL DISABLE Annoying) # Set warnings (if needed).
target_include_directories(${TEST_MAIN} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/CAEN")
target_link_directories(${TEST_MAIN} PRIVATE ${LIBRARY_OUTPUT_DIRECTORY})
target_link_libraries(${TEST_MAIN} CAENError CAENVME CAENComm ${DPPLIB} ${CMAKE_THREAD_LIBS_INIT} caenhvwrapper ${CMAKE_DL_LIBS} )

add_test(
    # Use some per-module/project prefix so that it is easier to run only tests for this module
    NAME ${TEST_MAIN}
    COMMAND ${TEST_MAIN} ${TEST_RUNNER_PARAMS})


