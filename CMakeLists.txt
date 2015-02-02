cmake_minimum_required(VERSION 2.8)

project (dtOO)

#------------------------------------------------------------------------------#
#                                                                              #
# macros                                                                       #
#                                                                              #
#------------------------------------------------------------------------------#
INCLUDE(cmake/dtOOHelperMacros.cmake)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

#------------------------------------------------------------------------------#
#                                                                              #
# externlibs path                                                              #
#                                                                              #
#------------------------------------------------------------------------------#
set(DTOO_EXTERNLIBS $ENV{EXTERNLIBS} CACHE FILEPATH "externlibs folder")
if (DTOO_EXTERNLIBS STREQUAL "")
  message( FATAL_ERROR "Please set DTOO_EXTERNLIBS." )
endif (DTOO_EXTERNLIBS STREQUAL "")

#------------------------------------------------------------------------------#
#
# compiler flags
#
#------------------------------------------------------------------------------#
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ldl") #for dladdr in Backtrace
#------------------------------------------------------------------------------#
#
# options
#
#------------------------------------------------------------------------------#
option(DTOO_USE_GOOGLEPERFTOOLS "Use GooglePerformance Tools" OFF)
option(DTOO_BUILDANDINSTALL_DOC "Use Doxygen to create the HTML based API documentation" OFF)

#------------------------------------------------------------------------------#
#
# required packages
#
#------------------------------------------------------------------------------#
find_package(Qt4 COMPONENTS QtCore QtXml REQUIRED)
find_package(CGAL REQUIRED)
find_package(GMSH REQUIRED)
find_package(
  OpenCASCADE 
  COMPONENTS 
  TKernel TKMath
  TKSTEP 
  TKFillet TKShHealing 
  TKTopAlgo TKBRep 
  TKGeomBase TKGeomAlgo 
  TKG3d TKG2d
  REQUIRED 
)
#TKMesh TKIGES TKXSBase TKOffset TKBool
#TKFeat TKSTEPAttr TKHLR TKBO TKPrim TKAdvTools 
#TKSTEP209 TKSTEPBase 

find_package(MUPARSER REQUIRED)
find_package(ROOT REQUIRED)
find_package(GSL REQUIRED)
find_package(
  Boost 
  COMPONENTS 
  filesystem 
  system 
  REQUIRED
)
find_package(CGNS REQUIRED)
if(DTOO_USE_GOOGLEPERFTOOLS)
  add_definitions(-DDTOO_USE_GOOGLEPROFTOOLS)
  find_package(GOOGLEPERFTOOLS REQUIRED)
endif(DTOO_USE_GOOGLEPERFTOOLS)


#------------------------------------------------------------------------------#
#
# make documentation
#
#------------------------------------------------------------------------------#
if (DTOO_BUILDANDINSTALL_DOC)
  CREATE_DTOO_DOC()
endif(DTOO_BUILDANDINSTALL_DOC)

#------------------------------------------------------------------------------#
#
# add directories
#
#------------------------------------------------------------------------------#
add_subdirectory(geometryKernel)
add_subdirectory(generalKernel)
add_subdirectory(libraryKernel)
add_subdirectory(meshKernel)
add_subdirectory(renderKernel)
add_subdirectory(pluginKernel)

#------------------------------------------------------------------------------#
#
# include directories
#
#------------------------------------------------------------------------------#
include_directories(
  geometryKernel 
  generalKernel 
  libraryKernel 
  meshKernel 
  renderKernel 
  pluginKernel
  ${Boost_INCLUDE_DIR} 
  ${CGAL_INCLUDE_DIR} 
  ${GMSH_INCLUDE_DIR} 
  ${MUPARSER_INCLUDE_DIR} 
  ${QT_QTCORE_INCLUDE_DIR}
  ${QT_QTXML_INCLUDE_DIR} 
  ${ROOT_INCLUDE_DIR} 
  ${GSL_INCLUDE_DIR} 
  ${OpenCASCADE_INCLUDE_DIR}
  ${CGNS_INCLUDE_DIR}
)

#------------------------------------------------------------------------------#
#
# parse version and write header version.h
#
#------------------------------------------------------------------------------#
MAKE_DTOO_VERSION(VERSION_MAJOR VERSION_MINOR VERSION_COMMIT)

#------------------------------------------------------------------------------#
#
# add target
#
#------------------------------------------------------------------------------#
add_library(dtOO SHARED ${DTOO_SOURCES})

#------------------------------------------------------------------------------#
#
# make shared library version
#
#------------------------------------------------------------------------------#
set(
  DTOO_VERSION_STRING 
  ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_COMMIT}
)

set_target_properties(
  dtOO 
  PROPERTIES 
  VERSION ${DTOO_VERSION_STRING} 
  SOVERSION ${VERSION_MAJOR}
)
add_dependencies(dtOO version)

#------------------------------------------------------------------------------#
#
# linking
#
#------------------------------------------------------------------------------#
target_link_libraries(dtOO 
  ${CGAL_LIBRARIES} 
  ${GMSH_LIBRARIES} 
  ${MUPARSER_LIBRARIES} 
  ${OpenCASCADE_LIBRARIES} 
  ${QT_QTCORE_LIBRARY}
  ${QT_QTXML_LIBRARY} 
  ${ROOT_LIBRARIES} 
  ${GSL_LIBRARIES}
  ${Boost_LIBRARIES}
  ${CGNS_LIBRARIES}
)
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS},-rpath,${CGAL_LINK_DIRECTORY}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS},-rpath,${GMSH_LINK_DIRECTORY}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS},-rpath,${MUPARSER_LINK_DIRECTORY}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS},-rpath,${OpenCASCADE_LINK_DIRECTORY}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS},-rpath,${ROOT_LINK_DIRECTORY}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS},-rpath,${CGNS_LINK_DIRECTORY}")

#------------------------------------------------------------------------------#
#
# install
#
#------------------------------------------------------------------------------#
install(TARGETS dtOO DESTINATION lib)
if (DTOO_BUILDANDINSTALL_DOC)
  install(DIRECTORY ${CMAKE_BINARY_DIR}/html DESTINATION doc)
endif (DTOO_BUILDANDINSTALL_DOC)
install(DIRECTORY pluginKernel/ DESTINATION include FILES_MATCHING PATTERN "*.h")
install(DIRECTORY meshKernel/ DESTINATION include FILES_MATCHING PATTERN "*.h")
install(DIRECTORY generalKernel/ DESTINATION include FILES_MATCHING PATTERN "*.h")
install(DIRECTORY libraryKernel/ DESTINATION include FILES_MATCHING PATTERN "*.h")
install(DIRECTORY geometryKernel/ DESTINATION include FILES_MATCHING PATTERN "*.h")
install(DIRECTORY renderKernel/ DESTINATION include FILES_MATCHING PATTERN "*.h")
install(DIRECTORY cmake/ DESTINATION cmake FILES_MATCHING PATTERN "*")
