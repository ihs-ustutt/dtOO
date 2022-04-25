cmake_minimum_required(VERSION 2.8)

project(designToolCoviseGUI)

#------------------------------------------------------------------------------#
#                                                                              #
# set DTOO_ROOTDIR and DTOO_EXTERNLIBS                                         #
#                                                                              #
#------------------------------------------------------------------------------#
if ("${DTOO_ROOTDIR}" STREQUAL "")
  message(FATAL_ERROR "DTOO_ROOTDIR is not defined or empty!")
endif ()

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
find_package( DTOO REQUIRED )

#------------------------------------------------------------------------------#
#                                                                              #
# include directories                                                          #
#                                                                              #
#------------------------------------------------------------------------------#
include_directories(
  ${DTOO_INCLUDE_DIR}
  ${DTOO_3RD_INCLUDE_DIRS}
  ${COVISE_INCLUDE_DIR} 
)

#------------------------------------------------------------------------------#
#                                                                              #
# compiler flags                                                               #
#                                                                              #
#------------------------------------------------------------------------------#
set(CMAKE_CXX_FLAGS ${DTOO_CXX_FLAGS})

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
  dtOOAbstractModule Qt5::Core Qt5::Xml
  coApi coAppl coCore coUtil coDo coShm 
  ${DTOO_LIBRARIES} ${DTOO_3RD_LIBRARIES}
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
  dtOOAbstractModule Qt5::Core Qt5::Xml
  coApi coAppl coCore coUtil coDo coShm
  ${DTOO_LIBRARIES} ${DTOO_3RD_LIBRARIES}
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
  dtOOAbstractModule Qt5::Core Qt5::Xml
  coApi coAppl coCore coUtil coDo coShm 
  ${DTOO_LIBRARIES} ${DTOO_3RD_LIBRARIES}
)

#------------------------------------------------------------------------------#
#                                                                              #
# RelAbs module                                                             #
#                                                                              #
#------------------------------------------------------------------------------#
set(RA_HEADERS
  relabs.h
)
set(RA_SOURCES
  relabs.cpp
)
add_executable(dtOOrelAbs ${RA_HEADERS} ${RA_SOURCES})
target_link_libraries(
  dtOOrelAbs 
  coApi coAppl coCore coUtil coDo coShm
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
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--no-undefined,--disable-new-dtags")
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
  TARGETS dtOODesignTool dtOOSimCaller dtOOrelAbs
  DESTINATION ${DTOO_COVISE_ARCHSUFFIX}/bin/${DTOO_COVISE_MODULE_FOLDER}
)
