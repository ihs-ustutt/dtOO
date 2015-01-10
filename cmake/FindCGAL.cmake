# - Find CGAL
# Find the CGAL includes and client library
# This module defines
# CGAL_DEFINITIONS: compiler flags for compiling with CGAL
# CGAL_INCLUDE_DIR: where to find CGAL.h
# CGAL_LIBRARIES: the libraries needed to use CGAL
# CGAL_FOUND: if false, do not try to use CGAL
#SET(CGAL_INCLUDE_DIR "not found")
#SET(CGAL_LIBRARIES "not found")

SET(CGAL_DEFINITIONS -frounding-math)
#
# prevents error assertion violation
#
ADD_DEFINITIONS(-DCGAL_DISABLE_ROUNDING_MATH_CHECK)

IF(CGAL_INCLUDE_DIR AND CGAL_LIBRARIES AND CGAL_LINK_DIRECTORY)
  SET(CGAL_FOUND TRUE)
ELSE(CGAL_INCLUDE_DIR AND CGAL_LIBRARIES AND CGAL_LINK_DIRECTORY)
  FIND_PATH(CGAL_INCLUDE_DIR CGAL
  ${DTOO_EXTERNLIBS}/cgal/include/
  /usr/include/CGAL
  /usr/local/include/CGAL
  $ENV{ProgramFiles}/CGAL/*/include/CGAL
  $ENV{SystemDrive}/CGAL/*/include/CGAL
  )
  FIND_PATH(CGAL_LINK_DIRECTORY libCGAL.so
    ${DTOO_EXTERNLIBS}/ALL/lib
  /usr/lib
  /usr/local/lib
  /usr/lib/CGAL
  /usr/lib64
  /usr/local/lib64
  /usr/lib64/CGAL
  $ENV{ProgramFiles}/CGAL/*/lib/ms
  $ENV{SystemDrive}/CGAL/*/lib/ms
  )

  FIND_LIBRARY(CGAL_LIBRARIES NAMES CGAL libCGAL
  PATHS
    ${DTOO_EXTERNLIBS}/cgal/lib
  /usr/lib
  /usr/local/lib
  /usr/lib/CGAL
  /usr/lib64
  /usr/local/lib64
  /usr/lib64/CGAL
  $ENV{ProgramFiles}/CGAL/*/lib/ms
  $ENV{SystemDrive}/CGAL/*/lib/ms
  )

  IF(CGAL_INCLUDE_DIR AND CGAL_LIBRARIES AND CGAL_LINK_DIRECTORY)
    SET(CGAL_FOUND TRUE)
    MESSAGE(STATUS "Found CGAL: ${CGAL_INCLUDE_DIR}, ${CGAL_LIBRARIES}, ${CGAL_LINK_DIRECTORY}")
  ELSE(CGAL_INCLUDE_DIR AND CGAL_LIBRARIES AND CGAL_LINK_DIRECTORY)
    SET(CGAL_FOUND FALSE)
    MESSAGE(STATUS "CGAL not found.")
  ENDIF(CGAL_INCLUDE_DIR AND CGAL_LIBRARIES AND CGAL_LINK_DIRECTORY)

  MARK_AS_ADVANCED(CGAL_INCLUDE_DIR CGAL_LIBRARIES CGAL_LINK_DIRECTORY)
ENDIF(CGAL_INCLUDE_DIR AND CGAL_LIBRARIES AND CGAL_LINK_DIRECTORY)
