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
add_executable(built4CubanLinx built4CubanLinx.cpp)

#
#
# linking
#
#
target_link_libraries(
  built4CubanLinx 
  dtOO
  ${Boost_LIBRARIES}
)

#
#
# add install rpath of dtOO lib
#
#
set(CMAKE_EXE_LINKER_FLAGS "-lreadline -Wl,--no-undefined")
set(
  CMAKE_EXE_LINKER_FLAGS 
  "${CMAKE_EXE_LINKER_FLAGS},-rpath,${CMAKE_INSTALL_PREFIX}/lib"
)

install(TARGETS built4CubanLinx DESTINATION bin)