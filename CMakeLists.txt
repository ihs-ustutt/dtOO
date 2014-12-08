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
OPTION(DTOO_USE_GOOGLEPERFTOOLS "Use GooglePerformance Tools" OFF)

#------------------------------------------------------------------------------#
#
# required packages
#
#------------------------------------------------------------------------------#
find_package(Qt4 COMPONENTS QtCore QtGui QtXml QtNetwork REQUIRED)
find_package(Qt4 COMPONENTS Qt3Support QtWebkit QtScript QtScriptTools QtUiTools)
find_package(CGAL REQUIRED)
find_package(GMSH REQUIRED)
find_package(OpenCASCADE COMPONENTS TKernel TKSTEP TKIGES TKXSBase TKOffset 
                                    TKFeat TKFillet TKBool TKShHealing TKMesh 
                                    TKHLR TKBO TKPrim TKTopAlgo TKBRep 
                                    TKGeomAlgo TKGeomBase TKG3d TKG2d 
                                    TKAdvTools TKMath TKernel TKSTEPAttr 
                                    TKSTEP209 TKSTEPBase)
find_package(MUPARSER REQUIRED)
find_package(ROOT)
find_package(GSL)
find_package(Boost COMPONENTS thread date_time program_options filesystem system REQUIRED)
IF(DTOO_USE_GOOGLEPERFTOOLS)
  ADD_DEFINITIONS(-DDTOO_USE_GOOGLEPROFTOOLS)
  FIND_PACKAGE(GOOGLEPERFTOOLS REQUIRED)
ENDIF(DTOO_USE_GOOGLEPERFTOOLS)

#------------------------------------------------------------------------------#
#
# make version
#
#------------------------------------------------------------------------------#
MAKE_DTOO_VERSION()

#------------------------------------------------------------------------------#
#
# make documentation
#
#------------------------------------------------------------------------------#
CREATE_DTOO_DOC()

#------------------------------------------------------------------------------#
#
# add directories
#
#------------------------------------------------------------------------------#
ADD_SUBDIRECTORY(geometryKernel)
ADD_SUBDIRECTORY(generalKernel)
ADD_SUBDIRECTORY(libraryKernel)
ADD_SUBDIRECTORY(meshKernel)
ADD_SUBDIRECTORY(renderKernel)
ADD_SUBDIRECTORY(pluginKernel)

#------------------------------------------------------------------------------#
#
# include directories
#
#------------------------------------------------------------------------------#
include_directories(
  geometryKernel generalKernel libraryKernel meshKernel renderKernel pluginKernel
  ${Boost_INCLUDE_DIR} 
  ${CGAL_INCLUDE_DIR} 
  ${GMSH_INCLUDE_DIR} 
  ${MUPARSER_INCLUDE_DIR} 
  ${QT_QTXML_INCLUDE_DIR} 
  ${QT_QTCORE_INCLUDE_DIR}
  ${ROOT_INCLUDE_DIR} 
  ${GSL_INCLUDE_DIR} 
  ${OpenCASCADE_INCLUDE_DIR}
)

#------------------------------------------------------------------------------#
#
# linking
#
#------------------------------------------------------------------------------#
add_library(dtOO SHARED ${DTOO_SOURCES})
add_dependencies(dtOO version)
target_link_libraries(dtOO 
  ${CGAL_LIBRARIES} 
  ${GMSH_LIBRARIES} 
  ${MUPARSER_LIBRARIES} 
  ${OpenCASCADE_LIBRARIES} 
  ${QT_QTXML_LIBRARY} 
  ${QT_QTCORE_LIBRARY}
  ${ROOT_LIBRARIES} 
  ${GSL_LIBRARIES}
)
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS},-rpath,${CGAL_LINK_DIRECTORY}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS},-rpath,${GMSH_LINK_DIRECTORY}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS},-rpath,${MUPARSER_LINK_DIRECTORY}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS},-rpath,${OpenCASCADE_LINK_DIRECTORY}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS},-rpath,${ROOT_LINK_DIRECTORY}")

#------------------------------------------------------------------------------#
#
# install
#
#------------------------------------------------------------------------------#
install(TARGETS dtOO DESTINATION lib)
install(DIRECTORY pluginKernel/ DESTINATION include/ FILES_MATCHING PATTERN "*.h")
install(DIRECTORY meshKernel/ DESTINATION include/ FILES_MATCHING PATTERN "*.h")
install(DIRECTORY generalKernel/ DESTINATION include/ FILES_MATCHING PATTERN "*.h")
install(DIRECTORY libraryKernel/ DESTINATION include/ FILES_MATCHING PATTERN "*.h")
install(DIRECTORY geometryKernel/ DESTINATION include/ FILES_MATCHING PATTERN "*.h")
install(DIRECTORY renderKernel/ DESTINATION include/ FILES_MATCHING PATTERN "*.h")
install(DIRECTORY cmake/ DESTINATION cmake/ FILES_MATCHING PATTERN "*")
