# - Find GMSH
# Find the GMSH includes and client library
# This module defines
# GMSH_DEFINITIONS: compiler flags for compiling with GMSH
# GMSH_INCLUDE_DIR: where to find GMSH.h
# GMSH_LIBRARIES: the libraries needed to use GMSH
# GMSH_FOUND: if false, do not try to use GMSH
#SET(GMSH_INCLUDE_DIR "not found")
#SET(GMSH_LIBRARIES "not found")

SET(GMSH_DEFINITIONS)

IF(GMSH_INCLUDE_DIR AND GMSH_LIBRARIES AND GMSH_LINK_DIRECTORY)
  SET(GMSH_FOUND TRUE)
ELSE(GMSH_INCLUDE_DIR AND GMSH_LIBRARIES AND GMSH_LINK_DIRECTORY)
  FIND_PATH(GMSH_INCLUDE_DIR gmsh/Gmsh.h
  $ENV{EXTERNLIBS}/gmsh/include/
  /usr/include/gmsh
  /usr/local/include/gmsh
  $ENV{ProgramFiles}/gmsh/*/include/gmsh
  $ENV{SystemDrive}/gmsh/*/include/gmsh
  )
  FIND_LIBRARY(GMSH_LIBRARIES NAMES Gmsh libGmsh
  PATHS
  $ENV{EXTERNLIBS}/gmsh/lib
  /usr/lib
  /usr/local/lib
  /usr/lib/gmsh
  /usr/lib64
  /usr/local/lib64
  /usr/lib64/gmsh
  $ENV{ProgramFiles}/gmsh/*/lib/ms
  $ENV{SystemDrive}/gmsh/*/lib/ms
  )

 FIND_PATH(GMSH_LINK_DIRECTORY libGmsh.so
    $ENV{EXTERNLIBS}/ALL/lib
    $ENV{DTOO_ROOTDIR}/lib
    /usr/lib
    /usr/local/lib
    /usr/lib/CGAL
    /usr/lib64
    /usr/local/lib64
    /usr/lib64/CGAL
  )

  IF(GMSH_INCLUDE_DIR AND GMSH_LIBRARIES AND GMSH_LINK_DIRECTORY)
    SET(GMSH_FOUND TRUE)
    MESSAGE(STATUS "Found GMSH: ${GMSH_INCLUDE_DIR}, ${GMSH_LIBRARIES}, ${GMSH_LINK_DIRECTORY}")
  ELSE(GMSH_INCLUDE_DIR AND GMSH_LIBRARIES AND GMSH_LINK_DIRECTORY)
    SET(GMSH_FOUND FALSE)
    MESSAGE(STATUS "GMSH not found.")
  ENDIF(GMSH_INCLUDE_DIR AND GMSH_LIBRARIES AND GMSH_LINK_DIRECTORY)

  MARK_AS_ADVANCED(GMSH_INCLUDE_DIR GMSH_LIBRARIES GMSH_LINK_DIRECTORY)
ENDIF(GMSH_INCLUDE_DIR AND GMSH_LIBRARIES AND GMSH_LINK_DIRECTORY)