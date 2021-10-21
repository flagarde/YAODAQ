if(BUILD_ROOT)
  include(ROOT)

  message(STATUS "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD ${STAGE1}")

  root_generate_dictionary(G__Channel "include/Channel.hpp" LINKDEF "include/ChannelLinkDef.hpp" ${STAGE1})
  add_library(Channel OBJECT "src/Channel.cpp" G__Channel.cxx)
  target_include_directories(Channel PUBLIC "${YAODAQ_INCLUDE}" PUBLIC "include")
  target_link_libraries(Channel PUBLIC ROOT::Core)
  set_target_properties(Channel PROPERTIES PUBLIC_HEADER "include/Channel.hpp")
  #install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libChannel_rdict.pcm TYPE BIN)
  install(TARGETS Channel)

  add_library(ChannelStatic STATIC $<TARGET_OBJECTS:Channel>)
  add_library(ChannelShared SHARED $<TARGET_OBJECTS:Channel>)

  root_generate_dictionary(G__Event "include/Event.hpp" LINKDEF "include/EventLinkDef.hpp" ${STAGE1})
  add_library(Event OBJECT "src/Event.cpp" G__Event.cxx)
  target_link_libraries(Event PUBLIC Channel)
  set_target_properties(Event PROPERTIES PUBLIC_HEADER "include/Event.hpp")
  #install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libEvent_rdict.pcm TYPE BIN)
  install(TARGETS Event)

  add_library(EventStatic STATIC $<TARGET_OBJECTS:Event>)
  target_link_libraries(EventStatic PUBLIC ChannelStatic)
  add_library(EventShared SHARED $<TARGET_OBJECTS:Event>)
  target_link_libraries(EventShared PUBLIC ChannelShared)

  add_library(DAQFile STATIC "src/DAQFile.cpp")
  target_link_libraries(DAQFile PUBLIC RootFile PUBLIC EventStatic PUBLIC ChannelStatic PUBLIC ROOT::Tree) #ADD ELOGPP
  target_include_directories(DAQFile PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)
  set_target_properties(DAQFile PROPERTIES PUBLIC_HEADER "include/DAQFile.hpp")
  install(TARGETS DAQFile)

  add_executable(DAQFileWritter DAQFileWritter.cpp)
  target_link_libraries(DAQFileWritter PRIVATE DAQFile PRIVATE FileWritter PRIVATE CLI11  PRIVATE ProgramInfos PUBLIC GeneralParameters)
  install(TARGETS DAQFileWritter)
endif()

add_executable(Digitizer Digitizer.cpp)
target_link_libraries(Digitizer PRIVATE CAENBoards PRIVATE Board PRIVATE CLI11 PRIVATE ProgramInfos PUBLIC GeneralParameters)
install(TARGETS Digitizer)
