include(LCIO_project)

set(LCIO_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(LCIO_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_library(LCIOFile STATIC "${LCIO_SRC}/LCIOFile.cpp")
add_dependencies(LCIOFile File YAODAQ::LCIO)
set_target_properties(LCIOFile PROPERTIES PUBLIC_HEADER "${LCIO_INCLUDE}/LCIOFile.hpp")
target_link_libraries(LCIOFile PUBLIC File PUBLIC YAODAQ::LCIO)
target_include_directories(LCIOFile PUBLIC $<BUILD_INTERFACE:${LCIO_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/LCIO>)
install(TARGETS LCIOFile PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/LCIO")

if(ENABLE_TESTS)
  add_subdirectory(tests)
endif()
