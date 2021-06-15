include(Net-snmp)

add_plugin(TARGET SNMPConnector VERSION ${YAODAQ_VERSION_MAJOR} BASE yaodaq::Connector CHILDS SNMP::SNMPConnector SOURCES "src/SNMPConnector.cpp" PUBLIC_HEADERS "include/SNMPConnector.hpp")
set_target_properties(SNMPConnector PROPERTIES PUBLIC_HEADER "include/SNMPConnector.hpp")
target_link_libraries(SNMPConnector PUBLIC Connector PUBLIC net-snmp::net-snmp)
target_include_directories(SNMPConnector PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/SNMP> PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
install(TARGETS SNMPConnector LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/SNMP")
