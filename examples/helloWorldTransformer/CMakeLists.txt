# define minimum cmake version
cmake_minimum_required(VERSION 2.6.2)

project(helloWorldTransformer)

find_path(
  DTOO_ROOTDIR cmake/dtOOHelperMacros.cmake
  PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(DTOO_EXTERNLIBS $ENV{EXTERNLIBS} CACHE FILEPATH "externlibs folder")
#if (DTOO_EXTERNLIBS STREQUAL "")
#  message( FATAL_ERROR "Please set DTOO_EXTERNLIBS." )
#endif (DTOO_EXTERNLIBS STREQUAL "")

INCLUDE("${DTOO_ROOTDIR}/cmake/dtOOHelperMacros.cmake")
set(
  CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${DTOO_ROOTDIR}/cmake/"
)

find_package(CGAL REQUIRED)
find_package(DTOO REQUIRED)

include_directories(
  ${CGAL_INCLUDE_DIR}
  ${DTOO_INCLUDE_DIR}
)

set(
  SOURCES 
  helloWorldTransformer.cpp helloWorldTransformer.h
)

add_library(helloWorldTransformer SHARED ${SOURCES})

