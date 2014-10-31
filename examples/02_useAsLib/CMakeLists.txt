cmake_minimum_required(VERSION 2.8)

project (dtOO)
set(CMAKE_BUILD_TYPE Release)

set(DTOOLIB_PATH "" CACHE PATH "installation directory of dtOO library")
set(DTOO_PATH "" CACHE PATH "source directory of dtOO library")

IF (NOT DTOOLIB_PATH AND NOT DTOO_PATH)
MESSAGE(FATAL_ERROR "Please set DTOOLIB_PATH and")
ENDIF (NOT DTOOLIB_PATH AND NOT DTOO_PATH)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${DTOO_PATH}/cmake/")

#FIND_PACKAGE(SISL REQUIRED)
FIND_PACKAGE(CGAL REQUIRED)
FIND_PACKAGE(GMSH REQUIRED)
#FIND_PACKAGE(MUPARSER REQUIRED)
FIND_PACKAGE(EO COMPONENTS eoutils eo es ga cma)
FIND_PACKAGE(OPENVOLUMEMESH REQUIRED COMPONENTS OpenVolumeMesh)
FIND_PACKAGE(COVISE COMPONENTS coApi coCore coUtil coShm REQUIRED)
find_package(Boost COMPONENTS thread date_time program_options filesystem system REQUIRED)

#ADD_SUBDIRECTORY(geometryKernel)
#ADD_SUBDIRECTORY(generalKernel)
#ADD_SUBDIRECTORY(libraryKernel)
#ADD_SUBDIRECTORY(meshKernel)
#ADD_SUBDIRECTORY(optimizationKernel)


include_directories(
  "${DTOOLIB_PATH}/include"
)

include_directories(
  geometryKernel generalKernel libraryKernel meshKernel optimizationKernel
  ${Boost_INCLUDE_DIR} ${CGAL_INCLUDE_DIR} ${EO_INCLUDE_DIR} ${GMSH_INCLUDE_DIR} 
  ${OPENVOLUMEMESH_INCLUDE_DIR}
  ${COVISE_INCLUDE_DIR}
)

add_executable(main main.cpp)
target_link_libraries(
  main "${DTOOLIB_PATH}/lib/libdtOO.so" ${COVISE_LIBRARIES} ${OPENVOLUMEMESH_LIBRARIES}
)

