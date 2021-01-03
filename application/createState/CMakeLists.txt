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
  ${Boost_LIBRARIES}
)
if (MPI_FOUND)
  target_link_libraries( createState ${MPI_LIBRARIES} ${MPI_CXX_LIBRARIES} )
endif (MPI_FOUND)

#
#
# add install rpath of dtOO lib
#
#
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--no-undefined")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS},-rpath,${CMAKE_INSTALL_PREFIX}/lib")

install(TARGETS createState DESTINATION bin)

message("**> Compile createState")