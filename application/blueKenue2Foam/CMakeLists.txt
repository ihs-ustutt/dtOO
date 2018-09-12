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
find_package(MPI REQUIRED)

#
#
# executable
#
#
add_executable(blueKenue2Foam blueKenue2Foam.cpp)

#
#
# linking
#
#
target_link_libraries(
  blueKenue2Foam 
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

install(TARGETS blueKenue2Foam DESTINATION bin)