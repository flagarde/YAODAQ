set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_CURRENT_SOURCE_DIR}/external")
include(serial)

set(SERIAL_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SERIAL_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src")

#add_library(SerialConnector SHARED "${SERIAL_SRC}/SerialConnector.cpp")
add_plugin(TARGET SerialConnector VERSION ${YAODAQ_VERSION_MAJOR} BASE Connector CHILDS Serial::SerialConnector SOURCES "${SERIAL_SRC}/SerialConnector.cpp" PUBLIC_HEADERS "${SERIAL_INCLUDE}/SerialConnector.hpp")
add_dependencies(SerialConnector YAODAQ::Serial)
#set_target_properties(SerialConnector PROPERTIES PUBLIC_HEADER "${SERIAL_INCLUDE}/SerialConnector.hpp")
target_link_libraries(SerialConnector PUBLIC Connector PUBLIC YAODAQ::Serial)
target_include_directories(SerialConnector PUBLIC $<BUILD_INTERFACE:${SERIAL_INCLUDE}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}/Serial>)
install(TARGETS SerialConnector RUNTIME DESTINATION "${RUNTIME_OUTPUT_DIR}" LIBRARY DESTINATION "${LIBRARY_OUTPUT_DIR}" ARCHIVE DESTINATION "${ARCHIVE_OUTPUT_DIR}" PUBLIC_HEADER DESTINATION "${INCLUDE_OUTPUT_DIR}/SNMP")


