#
#
# find packages
#
#
find_package(
  Boost 
  COMPONENTS 
  program_options
#  mpi
  REQUIRED
)
#find_package(MPI REQUIRED)

#
#
# executable
#
#
add_executable(createState createState.cpp)

#
#
# linking
#
#
target_link_libraries(
  createState 
  dtOO
#  ${MPI_CXX_LIBRARIES}
  ${Boost_LIBRARIES}
)

#
#
# add install rpath of dtOO lib
#
#
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--no-undefined")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS},-rpath,${CMAKE_INSTALL_PREFIX}/lib")

install(TARGETS createState DESTINATION bin)