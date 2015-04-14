cmake_minimum_required(VERSION 2.8)
#------------------------------------------------------------------------------#
#                                                                              #
# set DTOO_ROOTDIR and DTOO_EXTERNLIBS                                         #
#                                                                              #
#------------------------------------------------------------------------------#
find_path(DTOO_ROOTDIR include/progHelper.h
  $ENV{DTOO_ROOTDIR}
)
set(DTOO_EXTERNLIBS $ENV{EXTERNLIBS} CACHE FILEPATH "externlibs folder")
if (DTOO_EXTERNLIBS STREQUAL "")
  message( FATAL_ERROR "Please set DTOO_EXTERNLIBS." )
endif (DTOO_EXTERNLIBS STREQUAL "")

set(DTOO_COVISE_MODULE_FOLDER "designTool" CACHE STRING "covise folder")
set(DTOO_COVISE_ARCHSUFFIX $ENV{ARCHSUFFIX} CACHE STRING "covise archsuffix")

#------------------------------------------------------------------------------#
#                                                                              #
# add macros                                                                   #
#                                                                              #
#------------------------------------------------------------------------------#
message(STATUS "dtOO::> DTOO_ROOTDIR=${DTOO_ROOTDIR}")
include(${DTOO_ROOTDIR}/cmake/dtOOHelperMacros.cmake)
message(STATUS "dtOO::> cmake include ${DTOO_ROOTDIR}/cmake/dtOOHelperMacros.cmake")
file(GLOB USING_FILES "${DTOO_ROOTDIR}/cmake/Use*.cmake")
foreach(F ${USING_FILES})
   include(${F})
   message(STATUS "dtOO::> cmake include ${F}")
endforeach(F ${USING_FILES})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${DTOO_ROOTDIR}/cmake/")
message(STATUS "dtOO::> extent CMAKE_MODULE_PATH by ${DTOO_ROOTDIR}/cmake/")

set(CMAKE_MODULE_PATH "$ENV{COVISEDIR}/cmake" ${CMAKE_MODULE_PATH})
message(STATUS "dtOO::> extent CMAKE_MODULE_PATH by ${DTOO_ROOTDIR}/cmake/")


#------------------------------------------------------------------------------#
#                                                                              #
# find COVISE                                                                  #
#                                                                              #
#------------------------------------------------------------------------------#
find_package(COVISE REQUIRED)
#------------------------------------------------------------------------------#
#                                                                              #
# find dependencies                                                            #
#                                                                              #
#------------------------------------------------------------------------------#
find_package(CGAL)
add_definitions(-DCGAL_DISABLE_ROUNDING_MATH_CHECK)
USE_DTOO()

#------------------------------------------------------------------------------#
#                                                                              #
# include directories                                                          #
#                                                                              #
#------------------------------------------------------------------------------#
include_directories(
  ${BOOST_INCLUDE_DIR} 
  ${DTOO_INCLUDE_DIR} 
  ${CGAL_INCLUDE_DIR}
  ${COVISE_INCLUDE_DIR} 
)

#------------------------------------------------------------------------------#
#                                                                              #
# compiler flags                                                               #
#                                                                              #
#------------------------------------------------------------------------------#
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#------------------------------------------------------------------------------#
#                                                                              #
# static abstractModule library                                                #
#                                                                              #
#------------------------------------------------------------------------------#
set(AM_HEADERS
  coDoSetHandling.h
  abstractModule.h
)
set(AM_SOURCES
  coDoSetHandling.cpp
  abstractModule.cpp
)
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
)
add_library(dtOOAbstractModule STATIC ${AM_SOURCES} ${AM_HEADERS})
target_link_libraries(
  dtOOAbstractModule 
  coApi coAppl coCore coUtil coDo coShm ${EXTRA_LIBS}
)

#------------------------------------------------------------------------------#
#                                                                              #
# designTool module                                                            #
#                                                                              #
#------------------------------------------------------------------------------#
set(DT_HEADERS
  designTool.h
)
set(DT_SOURCES
  designTool.cpp
)
add_executable(dtOODesignTool ${DT_HEADERS} ${DT_SOURCES})
target_link_libraries(
  dtOODesignTool 
  dtOOAbstractModule coApi coAppl coCore coUtil coDo coShm ${EXTRA_LIBS}
)

#------------------------------------------------------------------------------#
#                                                                              #
# simCaller module                                                             #
#                                                                              #
#------------------------------------------------------------------------------#
set(SC_HEADERS
  simCaller.h
)
set(SC_SOURCES
  simCaller.cpp
)
add_executable(dtOOSimCaller ${SC_HEADERS} ${SC_SOURCES})
target_link_libraries(
  dtOOSimCaller 
  dtOOAbstractModule coApi coAppl coCore coUtil coDo coShm ${EXTRA_LIBS}
)

#------------------------------------------------------------------------------#
#                                                                              #
# linking                                                                      #
#                                                                              #
#------------------------------------------------------------------------------#
set(
  _rpathList
  ${DTOO_LINK_DIRECTORY}
  ${DTOO_EXTERNLIBS}/ALL/lib
)
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--no-undefined")
foreach(_rpath ${_rpathList})
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS},-rpath,${_rpath}")
  set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_RPATH}:${_rpath}")
endforeach(_rpath)

#------------------------------------------------------------------------------#
#                                                                              #
# install                                                                      #
#                                                                              #
#------------------------------------------------------------------------------#
set_target_properties(dtOODesignTool PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
set_target_properties(dtOOSimCaller PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)

install(
  TARGETS dtOODesignTool dtOOSimCaller 
  DESTINATION ${DTOO_COVISE_ARCHSUFFIX}/bin/${DTOO_COVISE_MODULE_FOLDER}
)
