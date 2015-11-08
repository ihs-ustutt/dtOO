#
#
# find packages
#
#
find_package(
  Boost 
  COMPONENTS 
  program_options
  REQUIRED
)

#
#
# executable
#
#
add_executable(listState listState.cpp)

#
#
# linking
#
#
target_link_libraries(
  listState 
  dtOO
  ${Boost_LIBRARIES}
)

#
#
# add install rpath of dtOO lib
#
#
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--no-undefined")
set(
  CMAKE_EXE_LINKER_FLAGS 
  "${CMAKE_EXE_LINKER_FLAGS},-rpath,${CMAKE_INSTALL_PREFIX}/lib"
)

#
#
# install application
#
#
install(TARGETS listState DESTINATION bin)