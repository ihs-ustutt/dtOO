# - Find EO
# Find the EO includes and client library
# This module defines
# EO_DEFINITIONS: compiler flags for compiling with EO
# EO_INCLUDE_DIR: where to find EO.h
# EO_LIBRARIES: the libraries needed to use EO
# EO_FOUND: if false, do not try to use EO
#SET(EO_INCLUDE_DIR "not found")
#SET(EO_LIBRARIES "not found")

SET(EO_DEFINITIONS )

IF( NOT EO_FOUND STREQUAL TRUE )
  FIND_PATH(EO_INCLUDE_DIR eo
    ${DTOO_EXTERNLIBS}/eo/include/eo
    /usr/include/EO
    /usr/local/include/EO
    $ENV{ProgramFiles}/EO/*/include/EO
    $ENV{SystemDrive}/EO/*/include/EO
  )
  IF( EO_INCLUDE_DIR STREQUAL eo-NOTFOUND )
    SET( EO_FOUND FALSE CACHE BOOL FORCE )
    MESSAGE( FATAL_ERROR "Cannot find EO include dir." )
  ENDIF( EO_INCLUDE_DIR STREQUAL eo-NOTFOUND )

  FIND_PATH(EO_LINK_DIRECTORY libeo.so
    PATHS
    ${DTOO_EXTERNLIBS}/eo/lib
    /usr/lib
    /usr/local/lib
    /usr/lib/EO
    /usr/lib64
    /usr/local/lib64
    /usr/lib64/EO
    $ENV{ProgramFiles}/EO/*/lib/ms
    $ENV{SystemDrive}/EO/*/lib/ms
  )
  IF( EO_LINK_DIRECTORY STREQUAL libeo.so-NOTFOUND )
    SET( EO_FOUND FALSE CACHE BOOL FORCE )
    MESSAGE( FATAL_ERROR "Cannot find EO lib dir." )
  ELSE( EO_LINK_DIRECTORY STREQUAL libeo.so-NOTFOUND )
    SET( EO_FOUND TRUE CACHE BOOL "Has EO been found?" FORCE )
    MESSAGE( STATUS "Found EO include dir: ${EO_INCLUDE_DIR}" )
    MESSAGE( STATUS "Found EO lib dir: ${EO_LINK_DIRECTORY}" )
  ENDIF( EO_LINK_DIRECTORY STREQUAL libeo.so-NOTFOUND )
ENDIF( NOT EO_FOUND STREQUAL TRUE )

  #  MESSAGE("EO_INCLUDE_DIR=${EO_INCLUDE_DIR}")
  #  MESSAGE("EO_LINK_DIRECTORY=${EO_LINK_DIRECTORY}")
  #  MESSAGE("EO_LIBRARIES=${EO_LIBRARIES}")
IF( EO_FOUND STREQUAL TRUE )
  IF (EO_LINK_DIRECTORY) 
    IF( DEFINED EO_FIND_COMPONENTS )
      FOREACH( _libname ${EO_FIND_COMPONENTS} )
        #look for libs in EO_LINK_DIRECTORY
        FIND_LIBRARY( ${_libname}_EOLIB ${_libname} ${EO_LINK_DIRECTORY} NO_DEFAULT_PATH)
        SET( _foundlib ${${_libname}_EOLIB} )
        IF( _foundlib STREQUAL ${_libname}_EOLIB-NOTFOUND )
          MESSAGE( FATAL_ERROR "Cannot find ${_libname}. Is it spelled correctly? Correct capitalization? Do you have another package with similarly-named libraries, installed at ${EO_LINK_DIRECTORY}? (That is where this script thinks the EO libs are.)" )
        ENDIF( _foundlib STREQUAL ${_libname}_EOLIB-NOTFOUND )
        SET( EO_LIBRARIES ${EO_LIBRARIES} ${_foundlib})# CACHE TYPE STRING FORCE)
      ENDFOREACH( _libname ${EO_FIND_COMPONENTS} )
    ENDIF( DEFINED EO_FIND_COMPONENTS )
  ENDIF (EO_LINK_DIRECTORY) 

  MARK_AS_ADVANCED(EO_INCLUDE_DIR EO_LIBRARIES)
ENDIF( EO_FOUND STREQUAL TRUE )
