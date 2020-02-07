# - Find DTOO
# Find the DTOO includes and client library
# This module defines
# DTOO_DEFINITIONS: compiler flags for compiling with DTOO
# DTOO_INCLUDE_DIR: where to find DTOO.h
# DTOO_LIBRARIES: the libraries needed to use DTOO
# DTOO_FOUND: if false, do not try to use DTOO
#SET(DTOO_INCLUDE_DIR "not found")
#SET(DTOO_LIBRARIES "not found")
IF(DTOO_INCLUDE_DIR AND DTOO_LIBRARIES AND DTOO_LINK_DIRECTORY)
  SET(DTOO_FOUND TRUE)
ELSE(DTOO_INCLUDE_DIR AND DTOO_LIBRARIES AND DTOO_LINK_DIRECTORY)
  FIND_PATH(DTOO_INCLUDE_DIR progHelper.h
    ${DTOO_ROOTDIR}/include
    /usr/include/
  )

  FIND_LIBRARY(DTOO_LIBRARIES NAMES dtOO libdtOO
    PATHS
    ${DTOO_ROOTDIR}/lib
    /usr/lib
    /usr/lib64
    /usr/local/lib
    /usr/local/lib64
  )

 FIND_PATH(DTOO_LINK_DIRECTORY libdtOO.so
    ${DTOO_ROOTDIR}/lib
    /usr/lib
    /usr/lib64
    /usr/local/lib
    /usr/local/lib64
  )

  if ( DTOO_ROOTDIR )
    if ( EXISTS "${DTOO_ROOTDIR}/cmake/dtOOConfig.cmake" )
      include( "${DTOO_ROOTDIR}/cmake/dtOOConfig.cmake" )
    endif()
  endif()


  IF(DTOO_INCLUDE_DIR AND DTOO_LIBRARIES AND DTOO_LINK_DIRECTORY)
    SET(DTOO_FOUND TRUE)
    MESSAGE(
      STATUS 
      "Found DTOO: \n \
       DTOO_INCLUDE_DIR=${DTOO_INCLUDE_DIR}, \n \
       DTOO_LIBRARIES=${DTOO_LIBRARIES}, \n \
       DTOO_LINK_DIRECTORY=${DTOO_LINK_DIRECTORY} \n \
       DTOO_3RD_INCLUDE_DIRS=${DTOO_3RD_INCLUDE_DIRS} \n \
       DTOO_3RD_LIBRARIES=${DTOO_3RD_LIBRARIES} \n \
       DTOO_3RD_LINKER_FLAGS=${DTOO_3RD_LINKER_FLAGS}"
    )

  ELSE(DTOO_INCLUDE_DIR AND DTOO_LIBRARIES AND DTOO_LINK_DIRECTORY)
    SET(DTOO_FOUND FALSE)
    MESSAGE(STATUS "DTOO not found.")
  ENDIF(DTOO_INCLUDE_DIR AND DTOO_LIBRARIES AND DTOO_LINK_DIRECTORY)
ENDIF(DTOO_INCLUDE_DIR AND DTOO_LIBRARIES AND DTOO_LINK_DIRECTORY)
