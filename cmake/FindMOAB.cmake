# - Find MOAB
# Find the MOAB includes and client library
# This module defines
# MOAB_DEFINITIONS: compiler flags for compiling with MOAB
# MOAB_INCLUDE_DIR: where to find MOAB.h
# MOAB_LIBRARIES: the libraries needed to use MOAB
# MOAB_FOUND: if false, do not try to use MOAB
#SET(MOAB_INCLUDE_DIR "not found")
#SET(MOAB_LIBRARIES "not found")

#SET(MOAB_DEFINITIONS)
IF(MOAB_INCLUDE_DIR AND MOAB_LIBRARIES AND MOAB_LINK_DIRECTORY)
  SET(MOAB_FOUND TRUE)
ELSE(MOAB_INCLUDE_DIR AND MOAB_LIBRARIES AND MOAB_LINK_DIRECTORY)
  FIND_PATH(MOAB_INCLUDE_DIR moab/Version.h
  ${DTOO_EXTERNLIBS}/moab/include/
  )
  FIND_LIBRARY(MOAB_LIBRARIES NAMES MOAB libMOAB
  PATHS
  ${DTOO_EXTERNLIBS}/moab/lib
  ${DTOO_EXTERNLIBS}/moab/lib64
  )

 FIND_PATH(MOAB_LINK_DIRECTORY libMOAB.so
    ${DTOO_EXTERNLIBS}/ALL/lib
    $ENV{DTOO_ROOTDIR}/lib
  )

  IF(MOAB_INCLUDE_DIR AND MOAB_LIBRARIES AND MOAB_LINK_DIRECTORY)
    SET(MOAB_FOUND TRUE)
    MESSAGE(STATUS "Found MOAB: ${MOAB_INCLUDE_DIR}, ${MOAB_LIBRARIES}, ${MOAB_LINK_DIRECTORY}")
  ELSE(MOAB_INCLUDE_DIR AND MOAB_LIBRARIES AND MOAB_LINK_DIRECTORY)
    SET(MOAB_FOUND FALSE)
    MESSAGE(STATUS "MOAB not found.")
  ENDIF(MOAB_INCLUDE_DIR AND MOAB_LIBRARIES AND MOAB_LINK_DIRECTORY)

  MARK_AS_ADVANCED(MOAB_INCLUDE_DIR MOAB_LIBRARIES MOAB_LINK_DIRECTORY)
ENDIF(MOAB_INCLUDE_DIR AND MOAB_LIBRARIES AND MOAB_LINK_DIRECTORY)