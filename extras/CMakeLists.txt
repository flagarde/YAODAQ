include(IterateFolder)

add_subdirectory(ROOT)

sub_dir_list(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})

foreach(subdir ${SUBDIRS})
  # set(CMAKE_MODULE_PATH "") ADD AN OPTION AND SET IT TO ON ONLY IF IT DOESN'T EXIST !"
  if(("${BUILD_${subdir}}" STREQUAL ""))
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/CMakeLists.txt")
      missive(ERROR "BUILD_${subdir} not in Setting.cmake ! By default I will compile !")
      add_subdirectory(${subdir})
    endif()
  elseif(("${BUILD_${subdir}}" STREQUAL "ON"))
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/CMakeLists.txt")
      missive(WARN "BUILD_${subdir} set to ${Green}ON${Default} in Setting.cmake")
      add_subdirectory(${subdir})
    endif()
  else()
    missive(WARN "BUILD_${subdir} set to ${Red}OFF${Default} in Setting.cmake")
  endif()
endforeach()
