find_package(Git REQUIRED)

include(CMakeParseArguments)

function(plugin)
  set(options OPTIONAL LINK)
  set(oneValueArgs TARGET VERSION BASE)
  set(multiValueArgs CHILDS)
  cmake_parse_arguments(plugin "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  get_target_property(VAR ${plugin_TARGET} PUBLIC_HEADER)
  if(WIN32)
    set(FT_EXPORT "__declspec(dllexport)")
  else()
    set(FT_EXPORT "__attribute__((visibility(\"default\")))")
  endif()

  foreach(name ${VAR})
    get_filename_component(buildDirRelFilePath "${name}" REALPATH BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
    list(APPEND INCLUDES "#include \"${buildDirRelFilePath}\"")
    string(REPLACE ";" "\n" INCLUDES "${INCLUDES}")
  endforeach()
  set(VERSION ${plugin_VERSION})
  set(REGISTERS "")
  set(LINK ${plugin_LINK})
  foreach(name ${plugin_CHILDS})
    list(
      APPEND
      REGISTERS
      "static_assert(std::is_base_of<${plugin_BASE},${name}>::value, \"ERROR: FLAKED_TUNA_PLUGIN: Registered ${name} must be of base type ${plugin_BASE}.\");\nstatic_assert((std::is_default_constructible<${name}>::value), \"ERROR: FLAKED_TUNA_PLUGIN: ${name} type is not default constructable.\");\npr->RegisterPlugin<${name},${plugin_BASE}>()"
      )
  endforeach()
  string(REPLACE ";" ";\n" REGISTERS "${REGISTERS}")
  list(APPEND REGISTERS "")
  configure_file("${CMAKE_SOURCE_DIR}/libs/src/PluginProto.cpp.in" "${CMAKE_BINARY_DIR}/generated/plugins/Plugin_${plugin_TARGET}.cpp" @ONLY)
  target_sources(${plugin_TARGET} PRIVATE "${CMAKE_BINARY_DIR}/generated/plugins/Plugin_${plugin_TARGET}.cpp")
endfunction()

include(IterateFolder)

sub_dir_list(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})

foreach(subdir ${SUBDIRS})
  set(CMAKE_MODULE_PATH "")
  ##ADD AN OPTION AND SET IT TO ON ONLY IF IT DOESNT EXIST !"
  if(("${BUILD_${subdir}}" STREQUAL ""))
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/CMakeLists.txt")
      message(AUTHOR_WARNING "BUILD_${subdir} not in Setting.cmake ! By default I will compile !")
      add_subdirectory(${subdir})
    endif()
  elseif(("${BUILD_${subdir}}" STREQUAL "ON"))
     if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/CMakeLists.txt")
      message(AUTHOR_WARNING "BUILD_${subdir} set to ${Green}ON${LastColor} in Setting.cmake")
      add_subdirectory(${subdir})
    endif()
  else()
    message(AUTHOR_WARNING "BUILD_${subdir} set to ${Red}OFF${LastColor} in Setting.cmake")
  endif()
endforeach()
