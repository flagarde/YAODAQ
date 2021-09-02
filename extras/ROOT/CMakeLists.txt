set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/external" "${CMAKE_SOURCE_DIR}/external")
include(ROOT)
set(ROOT_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(ROOT_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_library(RootFile STATIC "${ROOT_SRC}/RootFile.cpp")
set_target_properties(RootFile PROPERTIES PUBLIC_HEADER "${ROOT_INCLUDE}/RootFile.hpp")
target_link_libraries(RootFile PUBLIC File PUBLIC ROOT::Core PUBLIC ROOT::Tree)
target_include_directories(RootFile PUBLIC $<BUILD_INTERFACE:${ROOT_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/ROOT>)
install(TARGETS RootFile PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/ROOT")

if(ENABLE_TESTS)
  add_subdirectory(tests)
endif()
