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

cmake_minimum_required(VERSION 3.15)

option(DTOO_USE_CCACHE "Use CCACHE" OFF)
if (DTOO_USE_CCACHE)
  find_program(CCACHE_PROGRAM ccache)
  if(CCACHE_PROGRAM)
      set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
  endif()
endif(DTOO_USE_CCACHE)  

project(designTool)

SET( CMAKE_EXPORT_COMPILE_COMMANDS ON )
IF( EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" )
  EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
    ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
  )
ENDIF()

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
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ldl") #for dladdr in Backtrace
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lgmp") #for cgal
#add_compile_options("-fdiagnostics-color")

#------------------------------------------------------------------------------#
#
# options
#
#------------------------------------------------------------------------------#
option(DTOO_USE_MPI "Use MPI" OFF)
option(DTOO_USE_OMP "Use OpenMP" OFF)
option(DTOO_BUILD_DOC "Build documentation with doxygen" OFF)
#------------------------------------------------------------------------------#
#
# required packages
#
#------------------------------------------------------------------------------#
find_package(Qt5Core REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(NLOHMANN_JSON REQUIRED)
find_package(CGAL REQUIRED COMPONENTS CGAL)
find_package(GMSH REQUIRED gmsh)
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
find_package(MPFR)
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
  FOAMEXT
  COMPONENTS 
  foam meshTools dynamicMesh finiteVolume Pstream incompressibleRASModels
  triSurface decompositionMethods lagrangian incompressibleTurbulenceModel
  incompressibleTransportModels
)
if (FOAMEXT_FOUND)
  add_definitions(-DDTOO_HAS_FOAMEXT)
  find_package(OpenFoamTurbo COMPONENTS OpenFoamTurbo)
endif (FOAMEXT_FOUND)

if (NOT FOAMEXT_FOUND)
  find_package(
    OpenFOAM
    COMPONENTS 
    OpenFOAM meshTools surfMesh dynamicMesh finiteVolume Pstream 
    turbulenceModels
    incompressibleTransportModels
  )
  if (OpenFOAM_FOUND)
    add_definitions(-DDTOO_HAS_OpenFOAM)
      find_package(
        OpenFOAMMixingPlane
        COMPONENTS 
        mixingPlane
      )
      find_package(
        OpenFOAMMappedFieldFixedValue
        COMPONENTS
        mappedFieldFixedValue
      )
  endif (OpenFOAM_FOUND)
endif (NOT FOAMEXT_FOUND)

find_package(MOAB REQUIRED COMPONENTS MOAB)
find_package(OpenMesh REQUIRED COMPONENTS OpenMeshCore OpenMeshTools)
find_package(OpenVolumeMesh REQUIRED COMPONENTS OpenVolumeMesh)
if (DTOO_USE_OMP)
  find_package(OpenMP REQUIRED)
  message("**> Extent CMAKE_C_FLAGS by ${OpenMP_C_FLAGS}")
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  message("**> Extent CMAKE_CXX_FLAGS by ${OpenMP_CXX_FLAGS}")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  add_definitions(-DDTOO_HAS_OMP)
endif (DTOO_USE_OMP)

if (DTOO_USE_MPI)
  find_package(MPI REQUIRED)
endif (DTOO_USE_MPI)

#
# Use BOOST_CONFIG_LIBRARIES for configuring instead of Boost_LIBRARIES to 
# prevent error, when linking dtOODesignTool
#
if (MPI_FOUND)
  find_package(
    Boost 
    COMPONENTS 
    filesystem 
    system 
    timer
    program_options
    thread
    regex
    mpi
    serialization
    REQUIRED
  )
  SET(
    BOOST_CONFIG_LIBRARIES 
    "${Boost_FILESYSTEM_LIBRARY_RELEASE};${Boost_SYSTEM_LIBRARY_RELEASE};"
    "${Boost_TIMER_LIBRARY_RELEASE};${Boost_PROGRAM_OPTIONS_LIBRARY_RELEASE}"
    "${Boost_THREAD_LIBRARY_RELEASE};${Boost_REGEX_LIBRARY_RELEASE}"
    "${Boost_MPI_LIBRARY_RELEASE};${Boost_SERIALIZATION_LIBRARY_RELEASE}"
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
    thread
    regex
    REQUIRED
  )
  SET(
    BOOST_CONFIG_LIBRARIES 
    "${Boost_FILESYSTEM_LIBRARY_RELEASE};${Boost_SYSTEM_LIBRARY_RELEASE};"
    "${Boost_TIMER_LIBRARY_RELEASE};${Boost_PROGRAM_OPTIONS_LIBRARY_RELEASE}"
    "${Boost_THREAD_LIBRARY_RELEASE};${Boost_REGEX_LIBRARY_RELEASE}"
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
# special compile properties
#
#------------------------------------------------------------------------------#
set_property(
  SOURCE "src/libraryKernel/meshEngine/tetgen/tetgen.cxx" 
  PROPERTY COMPILE_FLAGS -DTETLIBRARY
)
if (OpenFOAM_FOUND)
  add_compile_options("-DWM_LABEL_SIZE=$ENV{WM_LABEL_SIZE}")
endif (OpenFOAM_FOUND)

#------------------------------------------------------------------------------#
#
# link directories
#
#------------------------------------------------------------------------------#
if (MPFR_FOUND)
  link_directories(
    ${MPFR_LINK_DIRECTORY}
  )
endif (MPFR_FOUND)

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
  ${MOAB_INCLUDE_DIR}
  ${OpenMesh_INCLUDE_DIR}
  ${OpenVolumeMesh_INCLUDE_DIR}
  ${IGL_INCLUDE_DIR}
  ${NLOHMANN_JSON_INCLUDE_DIR}
)

if (FOAMEXT_FOUND)
  include_directories(
    ${FOAMEXT_INCLUDE_DIR}/foam/lnInclude
    ${FOAMEXT_INCLUDE_DIR}/OSspecific/POSIX/lnInclude
    ${FOAMEXT_INCLUDE_DIR}/meshTools/lnInclude
    ${FOAMEXT_INCLUDE_DIR}/dynamicMesh/dynamicMesh/lnInclude
    ${FOAMEXT_INCLUDE_DIR}/finiteVolume/lnInclude
    ${FOAMEXT_INCLUDE_DIR}/turbulenceModels/incompressible/RAS/lnInclude
    ${FOAMEXT_INCLUDE_DIR}/transportModels/incompressible/lnInclude
    ${FOAMEXT_INCLUDE_DIR}/transportModels
    ${FOAMEXT_INCLUDE_DIR}/turbulenceModels
    ${FOAMEXT_INCLUDE_DIR}/turbulenceModels/incompressible/RAS/RASModel
  )
  if (OpenFoamTurbo_FOUND)
    include_directories( ${OpenFoamTurbo_INCLUDE_DIR} )
  endif (OpenFoamTurbo_FOUND)
else (FOAMEXT_FOUND)
  if (OpenFOAM_FOUND)
    include_directories(
      ${OpenFOAM_INCLUDE_DIR}/OpenFOAM/lnInclude
      ${OpenFOAM_INCLUDE_DIR}/OSspecific/POSIX/lnInclude
      ${OpenFOAM_INCLUDE_DIR}/meshTools/lnInclude
      ${OpenFOAM_INCLUDE_DIR}/dynamicMesh/lnInclude
      ${OpenFOAM_INCLUDE_DIR}/finiteVolume/lnInclude
      ${OpenFOAM_INCLUDE_DIR}/TurbulenceModels/turbulenceModels/lnInclude
      ${OpenFOAM_INCLUDE_DIR}/TurbulenceModels/incompressible/lnInclude
      ${OpenFOAM_INCLUDE_DIR}/transportModels/incompressible/lnInclude
      ${OpenFOAM_INCLUDE_DIR}/surfMesh/lnInclude
    )
    if (OpenFOAMMixingPlane_FOUND)
      include_directories( ${OpenFOAMMixingPlane_INCLUDE_DIR} )
    endif (OpenFOAMMixingPlane_FOUND)
    if (OpenFOAMMappedFieldFixedValue_FOUND)
      include_directories( ${OpenFOAMMappedFieldFixedValue_INCLUDE_DIR} )
    endif (OpenFOAMMappedFieldFixedValue_FOUND)
  endif (OpenFOAM_FOUND)
endif (FOAMEXT_FOUND)


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
# always compile logMe.cpp to have correct compile time in log file
add_custom_command(
  TARGET dtOO
  PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E touch_nocreate 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/generalKernel/logMe/logMe.cpp
)
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
  ${MOAB_LIBRARIES}
  ${OpenMesh_LIBRARIES}
  ${OpenVolumeMesh_LIBRARIES}
  ${IGL_LIBRARIES}
)

set(
  _configLibList
  ${CGAL_LIBRARIES} 
  ${GMSH_LIBRARIES} 
  ${MUPARSER_LIBRARIES} 
  ${OpenCASCADE_LIBRARIES} 
  ${ROOT_LIBRARIES} 
  ${GSL_LIBRARIES}
  ${BOOST_CONFIG_LIBRARIES}
  ${CGNS_LIBRARIES}
  ${MOAB_LIBRARIES}
  ${OpenMesh_LIBRARIES}
  ${OpenVolumeMesh_LIBRARIES}
)

set(
  _rpathList
  ${CGAL_LINK_DIRECTORY}
  ${GMSH_LINK_DIRECTORY} 
  ${MUPARSER_LINK_DIRECTORY}
  ${OpenCASCADE_LINK_DIRECTORY}
  ${ROOT_LINK_DIRECTORY}
  ${CGNS_LINK_DIRECTORY}
  ${MOAB_LINK_DIRECTORY}
  ${OpenVolumeMesh_LINK_DIRECTORY}
)

if (FOAMEXT_FOUND)
  target_link_libraries(
    dtOO 
    ${FOAMEXT_LIBRARIES}
  )

  set(
    _configLibList 
    "${_configLibList};${FOAMEXT_LIBRARIES}"
  )

  set(
    _rpathList
    "${_rpathList};${FOAMEXT_LINK_DIRECTORY};"
    "${FOAMEXT_LINK_DIRECTORY}/openmpi-system"
  )
  if (OpenFoamTurbo_FOUND)
    target_link_libraries(
      dtOO 
      ${OpenFoamTurbo_LIBRARIES}
    )

    set(
      _configLibList 
      "${_configLibList};${OpenFoamTurbo_LIBRARIES}"
    )

    set(
      _rpathList
      "${_rpathList};${OpenFoamTurbo_LINK_DIRECTORY}"
    )
  endif (OpenFoamTurbo_FOUND)
endif (FOAMEXT_FOUND)

if (OpenFOAM_FOUND)
  target_link_libraries(
    dtOO 
    ${OpenFOAM_LIBRARIES}
  )

  set(
    _configLibList 
    "${_configLibList};${OpenFOAM_LIBRARIES}"
  )

  set(
    _rpathList
    "${_rpathList};${OpenFOAM_LINK_DIRECTORY};"
    "${OpenFOAM_LINK_DIRECTORY}/openmpi-system"
  )

    if (OpenFOAMMixingPlane_FOUND)
    target_link_libraries(
      dtOO 
      ${OpenFOAMMixingPlane_LIBRARIES}
    )
    set(
      _configLibList "${_configLibList};${OpenFOAMMixingPlane_LIBRARIES}"
    )
    set(
      _rpathList "${_rpathList};${OpenFOAMMixingPlane_LINK_DIRECTORY}"
    )      
    endif (OpenFOAMMixingPlane_FOUND)
    if (OpenFOAMMappedFieldFixedValue_FOUND)
    target_link_libraries(
      dtOO
      ${OpenFOAMMappedFieldFixedValue_LIBRARIES}
    )
    set(
      _configLibList "${_configLibList};${OpenFOAMMappedFieldFixedValue_LIBRARIES}"
    )
    set(
      _rpathList "${_rpathList};${OpenFOAMMappedFieldFixedValue_LINK_DIRECTORY}"
    )
    endif (OpenFOAMMappedFieldFixedValue_FOUND)
endif (OpenFOAM_FOUND)

if (MPI_FOUND)
  target_link_libraries( dtOO ${MPI_LIBRARIES} ${MPI_CXX_LIBRARIES} )
  set(_configLibList "${_configLibList};${MPI_CXX_LIBRARIES};${MPI_LIBRARIES}")
  set(_rpathList "${_rpathList};${MPI_CXX_LIBRARIES};${MPI_LIBRARIES}")
endif (MPI_FOUND)

list(REMOVE_DUPLICATES _configLibList)
list(REMOVE_DUPLICATES _rpathList)

set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined,--disable-new-dtags")
foreach(_rpath ${_rpathList})
  message(STATUS "RPATH: Adding ${_rpath}")
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
  ${IGL_INCLUDE_DIR}
  ${Qt5Core_INCLUDE_DIRS}
  ${Qt5Xml_INCLUDE_DIRS}
  ${NLOHMANN_JSON_INCLUDE_DIR}
)
set(
  CONF_DTOO_3RD_LIBRARIES 
  ${_configLibList}
)
set(
  CONF_DTOO_CXX_FLAGS 
  "-ldl -lgmp -DCGAL_DISABLE_ROUNDING_MATH_CHECK"
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
  FILES_MATCHING 
  PATTERN "[a-zA-Z]*"
  PATTERN "__init__.py"
  PATTERN "__pycache__" EXCLUDE
  PATTERN ".spyproject" EXCLUDE
)

#
# add applications
#
add_subdirectory(application)

#
# add tools
#
add_subdirectory(tools)

#
# add test
#
enable_testing()
add_subdirectory(test)

#
# add doc
#
add_subdirectory(doc)
