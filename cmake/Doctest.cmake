include(doctest)

if(ENABLE_DOCTESTS)
  add_definitions(-DENABLE_DOCTEST_IN_LIBRARY)
endif()

# Expose required variable (DOCTEST_INCLUDE_DIR) to parent scope
ExternalProject_Get_Property(doctest source_dir)

set(DOCTEST_INCLUDE_DIR
    ${source_dir}/doctest
    CACHE INTERNAL "Path to include folder for doctest")
include_directories(doctest INTERFACE ${DOCTEST_INCLUDE_DIR})
