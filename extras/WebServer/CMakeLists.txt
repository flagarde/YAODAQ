set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external")

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/www/" DESTINATION "${CMAKE_INSTALL_PREFIX}/www/")

include(jsroot)
include(civet)
