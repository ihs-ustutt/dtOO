cmake_minimum_required(VERSION 2.8)

project (designTool)

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
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ldl") #for dladdr in Backtrace
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lgmp") #for cgal
#------------------------------------------------------------------------------#
#
# options
#
#------------------------------------------------------------------------------#
option(DTOO_USE_MPI "Use MPI" ON)

#------------------------------------------------------------------------------#
#
# required packages
#
#------------------------------------------------------------------------------#
find_package(Qt5Core REQUIRED)
find_package(Qt5Xml REQUIRED)

find_package(CGAL REQUIRED COMPONENTS CGAL)
find_package(GMSH REQUIRED Gmsh)
find_package(
  OpenCASCADE 
  COMPONENTS 
  TKernel TKMath
  TKSTEP TKIGES
  TKFillet TKShHealing 
  TKTopAlgo TKBRep 
  TKGeomBase TKGeomAlgo 
  TKG3d TKG2d
  TKXSBase
  REQUIRED 
)
#TKMesh TKIGES TKXSBase TKOffset TKBool
#TKFeat TKSTEPAttr TKHLR TKBO TKPrim TKAdvTools 
#TKSTEP209 TKSTEPBase 

find_package(MUPARSER REQUIRED muparser)
find_package(
  ROOT 
  REQUIRED 
  COMPONENTS
  Core Matrix MathCore MathMore Minuit2 Thread Hist
)
find_package(GSL REQUIRED)
find_package(CGNS REQUIRED COMPONENTS cgns)
find_package(
  OpenFOAM
  COMPONENTS 
  foam meshTools dynamicMesh finiteVolume Pstream incompressibleRASModels
  triSurface decompositionMethods lagrangian incompressibleTurbulenceModel
  incompressibleTransportModels
)
find_package(OpenFoamTurbo COMPONENTS OpenFoamTurbo)
find_package(CGM REQUIRED COMPONENTS iGeom)
find_package(MOAB REQUIRED COMPONENTS MOAB iMesh)
find_package(OpenMesh REQUIRED COMPONENTS OpenMeshCore OpenMeshTools)
find_package(OpenVolumeMesh REQUIRED COMPONENTS OpenVolumeMesh)
find_package(OpenMP REQUIRED)
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

if (DTOO_USE_MPI)
  find_package(MPI)
endif (DTOO_USE_MPI)

if (MPI_FOUND)
  find_package(
    Boost 
    COMPONENTS 
    filesystem 
    system 
    timer
    program_options
    mpi
    serialization
    REQUIRED
  )
  add_definitions(-DDTOO_HAS_MPI)
else (MPI_FOUND)
  find_package(
    Boost 
    COMPONENTS 
    filesystem 
    system 
    timer
    program_options
    REQUIRED
  )
endif (MPI_FOUND)

#------------------------------------------------------------------------------#
#
# add src directory
#
#------------------------------------------------------------------------------#
add_subdirectory(src)

#------------------------------------------------------------------------------#
#
# include directories
#
#------------------------------------------------------------------------------#
include_directories(
  src/geometryKernel 
  src/generalKernel 
  src/libraryKernel 
  src/meshKernel 
  src/renderKernel 
  src/pluginKernel
  src/solverKernel
  src/applicationKernel
  ${Boost_INCLUDE_DIR} 
  ${CGAL_INCLUDE_DIR} 
  ${GMSH_INCLUDE_DIR} 
  ${MUPARSER_INCLUDE_DIR}
  ${Qt5Core_INCLUDE_DIRS}
  ${Qt5Xml_INCLUDE_DIRS}
  ${ROOT_INCLUDE_DIR} 
  ${GSL_INCLUDE_DIR} 
  ${OpenCASCADE_INCLUDE_DIR}
  ${CGNS_INCLUDE_DIR}
  ${OpenFOAM_INCLUDE_DIR}/foam/lnInclude
  ${OpenFOAM_INCLUDE_DIR}/OSspecific/POSIX/lnInclude
  ${OpenFOAM_INCLUDE_DIR}/meshTools/lnInclude
  ${OpenFOAM_INCLUDE_DIR}/dynamicMesh/dynamicMesh/lnInclude
  ${OpenFOAM_INCLUDE_DIR}/finiteVolume/lnInclude
  ${OpenFOAM_INCLUDE_DIR}/turbulenceModels/incompressible/RAS/lnInclude
  ${OpenFOAM_INCLUDE_DIR}/transportModels/incompressible/lnInclude
  ${OpenFOAM_INCLUDE_DIR}/transportModels
  ${OpenFOAM_INCLUDE_DIR}/turbulenceModels
  ${OpenFOAM_INCLUDE_DIR}/turbulenceModels/incompressible/RAS/RASModel
  ${CGM_INCLUDE_DIR}
  ${MOAB_INCLUDE_DIR}
  ${OpenMesh_INCLUDE_DIR}
  ${OpenVolumeMesh_INCLUDE_DIR}
  ${OpenFoamTurbo_INCLUDE_DIR}
)

if (MPI_FOUND)
  include_directories(
    AFTER 
    ${MPI_CXX_INCLUDE_PATH} 
)
endif (MPI_FOUND)

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
target_link_libraries(
  dtOO 
  ${CGAL_LIBRARIES} 
  ${GMSH_LIBRARIES} 
  ${MUPARSER_LIBRARIES} 
  ${OpenCASCADE_LIBRARIES} 
  Qt5::Core
  Qt5::Xml
  ${ROOT_LIBRARIES} 
  ${GSL_LIBRARIES}
  ${Boost_LIBRARIES}
  ${CGNS_LIBRARIES}
  ${OpenFOAM_LIBRARIES}
  ${CGM_LIBRARIES}
  ${MOAB_LIBRARIES}
  ${OpenMesh_LIBRARIES}
  ${OpenVolumeMesh_LIBRARIES}
  ${OpenFoamTurbo_LIBRARIES}
)

set(
  _configLibList
  ${CGAL_LIBRARIES} 
  ${GMSH_LIBRARIES} 
  ${MUPARSER_LIBRARIES} 
  ${OpenCASCADE_LIBRARIES} 
  ${ROOT_LIBRARIES} 
  ${GSL_LIBRARIES}
  ${Boost_LIBRARIES}
  ${CGNS_LIBRARIES}
  ${OpenFOAM_LIBRARIES}
  ${CGM_LIBRARIES}
  ${MOAB_LIBRARIES}
  ${OpenMesh_LIBRARIES}
  ${OpenVolumeMesh_LIBRARIES}
  ${OpenFoamTurbo_LIBRARIES}
)

set(
  _rpathList
  ${CGAL_LINK_DIRECTORY}
  ${GMSH_LINK_DIRECTORY} 
  ${MUPARSER_LINK_DIRECTORY}
  ${OpenCASCADE_LINK_DIRECTORY}
  ${ROOT_LINK_DIRECTORY}
  ${CGNS_LINK_DIRECTORY}
  ${OpenFOAM_LINK_DIRECTORY}
  ${OpenFOAM_LINK_DIRECTORY}/openmpi-system
  ${CGM_LINK_DIRECTORY}
  ${MOAB_LINK_DIRECTORY}
  ${OpenVolumeMesh_LINK_DIRECTORY}
  ${OpenFoamTurbo_LINK_DIRECTORY}
)

if (MPI_FOUND)
  target_link_libraries( dtOO ${MPI_LIBRARIES} ${MPI_CXX_LIBRARIES} )
  set(_configLibList "${_configLibList};${MPI_CXX_LIBRARIES};${MPI_LIBRARIES}")
  set(_rpathList "${_rpathList};${MPI_CXX_LIBRARIES};${MPI_LIBRARIES}")
endif (MPI_FOUND)

list(REMOVE_DUPLICATES _configLibList)
list(REMOVE_DUPLICATES _rpathList)

set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined")
foreach(_rpath ${_rpathList})
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS},-rpath,${_rpath}")
  set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_RPATH}:${_rpath}")
endforeach(_rpath)

#------------------------------------------------------------------------------#
#
# install
#
#------------------------------------------------------------------------------#
set_target_properties(dtOO PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)

set(
  CONF_DTOO_3RD_INCLUDE_DIRS 
  ${CGAL_INCLUDE_DIR}
  ${ROOT_INCLUDE_DIR}
  ${Boost_INCLUDE_DIR}
)
set(
  CONF_DTOO_3RD_LIBRARIES 
  ${_configLibList}
)
set(
  CONF_DTOO_CXX_FLAGS 
  "-std=c++11 -ldl -lgmp -DCGAL_DISABLE_ROUNDING_MATH_CHECK"
)
set(
  CONF_DTOO_3RD_LINKER_FLAGS 
  "${CMAKE_SHARED_LINKER_FLAGS}"
)

if (MPI_FOUND)
  set(
    CONF_DTOO_3RD_INCLUDE_DIRS 
    "${CONF_DTOO_3RD_INCLUDE_DIRS};${MPI_CXX_INCLUDE_PATH}"
  )
  set(
    CONF_DTOO_3RD_LIBRARIES 
    "${CONF_DTOO_3RD_LIBRARIES};${MPI_CXX_LIBRARIES}"
  )
  set(
    CONF_DTOO_CXX_FLAGS 
    "${CONF_DTOO_CXX_FLAGS} -DDTOO_HAS_MPI"
  )
endif (MPI_FOUND)

configure_file(
  ${CMAKE_SOURCE_DIR}/cmake/dtOOConfig.cmake.in
  ${CMAKE_SOURCE_DIR}/cmake/dtOOConfig.cmake @ONLY
)

install(TARGETS dtOO DESTINATION lib)
install(DIRECTORY src/pluginKernel/ DESTINATION include FILES_MATCHING PATTERN "*.h")
install(DIRECTORY src/meshKernel/ DESTINATION include FILES_MATCHING PATTERN "*.h")
install(DIRECTORY src/generalKernel/ DESTINATION include FILES_MATCHING PATTERN "*.h")
install(DIRECTORY src/libraryKernel/ DESTINATION include FILES_MATCHING PATTERN "*.h")
install(DIRECTORY src/geometryKernel/ DESTINATION include FILES_MATCHING PATTERN "*.h")
install(DIRECTORY src/renderKernel/ DESTINATION include FILES_MATCHING PATTERN "*.h")
install(DIRECTORY src/solverKernel/ DESTINATION include FILES_MATCHING PATTERN "*.h")
install(DIRECTORY src/applicationKernel/ DESTINATION include FILES_MATCHING PATTERN "*.h")
install(DIRECTORY cmake/ DESTINATION cmake FILES_MATCHING PATTERN "*")
install(
  DIRECTORY scripts/ 
  DESTINATION scripts 
  FILES_MATCHING PATTERN "*"
)

#
# add applications
#
add_subdirectory(application)

#
# add tools
#
add_subdirectory(tools)