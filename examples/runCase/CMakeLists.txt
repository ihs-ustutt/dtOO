cmake_minimum_required(VERSION 2.8)

project (runCase)

set(DTOO_ROOTDIR CACHE FILEPATH "rootdir folder")
if (DTOO_ROOTDIR STREQUAL "")
  message( FATAL_ERROR "Please set DTOO_ROOTDIR." )
endif (DTOO_ROOTDIR STREQUAL "")

set(DTOO_EXTERNLIBS $ENV{EXTERNLIBS} CACHE FILEPATH "externlibs folder")
if (DTOO_EXTERNLIBS STREQUAL "")
  message( FATAL_ERROR "Please set DTOO_EXTERNLIBS." )
endif (DTOO_EXTERNLIBS STREQUAL "")

set(
  CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${DTOO_ROOTDIR}/cmake/"
)

#
#
# find packages
#
#
find_package(DTOO REQUIRED)
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
# include directories
#
#
include_directories( 
  ${DTOO_INCLUDE_DIR} ${Boost_INCLUDE_DIR} ${MPI_CXX_INCLUDE_PATH} 
)

foreach(_anInc ${DTOO_3RD_INCLUDE_DIRS})
  include_directories( AFTER ${_anInc} )
endforeach(_anInc)

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
  ${DTOO_LIBRARIES}
  ${MPI_CXX_LIBRARIES}
  ${Boost_LIBRARIES}
)

foreach(_aLib ${DTOO_3RD_LIBRARIES})
  target_link_libraries( runCase ${_aLib} )
endforeach(_aLib)

#
#
# compile and link flags
#
#
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${DTOO_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${DTOO_3RD_LINKER_FLAGS}")

set_target_properties(runCase PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)

install(TARGETS runCase DESTINATION bin)