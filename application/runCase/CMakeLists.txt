#------------------------------------------------------------------------------
#  dtOO < design tool Object-Oriented >
#    
#    Copyright (C) 2024 A. Tismer.
#------------------------------------------------------------------------------
#License
#    This file is part of dtOO.
#
#    dtOO is distributed in the hope that it will be useful, but WITHOUT
#    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
#    dtOO root directory for more details.
#
#    You should have received a copy of the License along with dtOO.
#
#------------------------------------------------------------------------------

if (NOT DTOO_USE_MPI)
  return()
endif (NOT DTOO_USE_MPI)

#
#
# find packages
#
#
find_package(
  Boost 
  COMPONENTS 
  program_options
  mpi
  REQUIRED
)
find_package(MPI REQUIRED)

#
#
# executable
#
#
add_executable(runCase runCase.cpp)

#
#
# linking
#
#
target_link_libraries(
  runCase 
  dtOO
  ${MPI_CXX_LIBRARIES}
  ${Boost_LIBRARIES}
)

#
#
# add install rpath of dtOO lib
#
#
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--no-undefined")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS},-rpath,${CMAKE_INSTALL_PREFIX}/lib")

install(TARGETS runCase DESTINATION bin)

message("**> Compile runCase")
