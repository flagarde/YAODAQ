include(CodeCoverage)

add_executable(Exceptions Exceptions.cpp)
add_dependencies(Exceptions doctest_with_main Exception)
set_target_properties(Exceptions PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX})
target_include_directories(Exceptions PRIVATE "${INCLUDE_OUTPUT_DIR}")
target_link_libraries(Exceptions PRIVATE Exception PRIVATE doctest_with_main)
doctest_discover_tests(Exceptions COMMAND Exceptions ADD_LABELS "Exceptions")
