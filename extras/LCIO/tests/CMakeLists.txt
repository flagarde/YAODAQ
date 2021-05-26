add_executable(LCIOTests LCIOFile.cpp)

set_target_properties(LCIOTests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX})
target_include_directories(LCIOTests PUBLIC "${INCLUDE_OUTPUT_DIR}")

target_link_libraries(LCIOTests PUBLIC LCIOFile PUBLIC doctestMain)
doctest_discover_tests(LCIOTests COMMAND LCIOTests)
