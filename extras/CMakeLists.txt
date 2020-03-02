find_package(Git REQUIRED)

find_package(Threads)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

include(IterateFolder)

sub_dir_list(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})

foreach(subdir ${SUBDIRS})
  if(EXISTS ${subdir}/CMakeLists.txt)
    message(STATUS "Entering in folder ${subdir}")
    add_subdirectory(${subdir})
  endif()
endforeach()

include(CMakeParseArguments)

function(plugin)
  set(options OPTIONAL FAST)
  set(oneValueArgs TARGET VERSION BASE)
  set(multiValueArgs CHILDS)
  cmake_parse_arguments(plugin "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  get_target_property(VAR ${plugin_TARGET} PUBLIC_HEADER)

  foreach(name ${VAR})
    message(STATUS "KKKK ${name}")
    get_filename_component(buildDirRelFilePath "${name}" REALPATH BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
    list(APPEND INCLUDES "#include \"${buildDirRelFilePath}\"")
    string(REPLACE ";" "\n" INCLUDES "${INCLUDES}")
  endforeach()
  set(VERSION ${plugin_VERSION})
  set(REGISTERS "")
  foreach(name ${plugin_CHILDS})
    list(APPEND REGISTERS "FLAKED_TUNA_CHECKS(${name},${plugin_BASE})\npr->RegisterPlugin<${name},${plugin_BASE}>()")
  endforeach()
  string(REPLACE ";" ";\n" REGISTERS "${REGISTERS}")
  list(APPEND REGISTERS "")
  configure_file("${CMAKE_SOURCE_DIR}/libs/src/PluginProto.cpp.in" "${CMAKE_BINARY_DIR}/generated/plugins/Plugin_${plugin_TARGET}.cpp" @ONLY)
  message(STATUS ${REGISTERS})
  target_sources(${plugin_TARGET} PRIVATE "${CMAKE_BINARY_DIR}/generated/plugins/Plugin_${plugin_TARGET}.cpp")
endfunction()

add_subdirectory(CAEN)
