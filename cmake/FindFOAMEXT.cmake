# - Try to find foam-ext libraries
### Does not test what version has been found,though
### that could be done by parsing Standard_Version.hxx


# Once done, this will define
# FOAMEXT_FOUND - true if OF has been found
# FOAMEXT_INCLUDE_DIR - the OF include dir
# FOAMEXT_LIBRARIES - names of OF libraries
# FOAMEXT_LINK_DIRECTORY - location of OF libraries

# ${FOAMEXT_FOUND} is cached, so once OF is found this block shouldn't have to run again
IF( NOT FOAMEXT_FOUND STREQUAL TRUE )
  set( _incsearchpath $ENV{FOAM_SRC})
  set( _testlibname libODE.so )
  set( _libsearchpath $ENV{FOAM_LIBBIN} )
  #find the include dir by looking for Standard_Real.hxx
  FIND_PATH( 
    FOAMEXT_INCLUDE_DIR 
    foam 
    PATHS ${_incsearchpath} 
    DOC "Path to FE includes" 
    NO_DEFAULT_PATH
  )

  # Find one lib and save its directory to FOAMEXT_LINK_DIRECTORY. Because
  # OF has so many libs, there is increased risk of a name collision.
  # Requiring that all libs be in the same directory reduces the risk.
  FIND_PATH( 
    FOAMEXT_LINK_DIRECTORY 
    ${_testlibname} 
    PATH 
    ${_libsearchpath} 
    DOC 
    "Path to FE libs" 
  )
  file( 
    GLOB FOAMEXT_OPENMPI_DIR
    "${FOAMEXT_LINK_DIRECTORY}/openmpi*"
  )
  IF( FOAMEXT_LINK_DIRECTORY MATCHES ".*NOTFOUND.*" )
    SET( FOAMEXT_FOUND FALSE CACHE BOOL "Has OF been found?" FORCE )
    MESSAGE( WARNING "Cannot find FE lib dir. Install foam-ext." )
  ELSE( FOAMEXT_LINK_DIRECTORY MATCHES ".*NOTFOUND.*" )
    SET( FOAMEXT_FOUND TRUE CACHE BOOL "Has OF been found?" FORCE )
    MESSAGE( STATUS "Found FE include dir: ${FOAMEXT_INCLUDE_DIR}" )
    MESSAGE( STATUS "Found FE lib dir: ${FOAMEXT_LINK_DIRECTORY}" )
  ENDIF( FOAMEXT_LINK_DIRECTORY MATCHES ".*NOTFOUND.*" )
ENDIF( NOT FOAMEXT_FOUND STREQUAL TRUE )

IF( FOAMEXT_FOUND STREQUAL TRUE )
  IF( DEFINED FOAMEXT_FIND_COMPONENTS )
    FOREACH( _libname ${FOAMEXT_FIND_COMPONENTS} )
      #look for libs in FOAMEXT_LINK_DIRECTORY
      FIND_LIBRARY( 
        ${_libname}_OFLIB 
        ${_libname} 
        ${FOAMEXT_LINK_DIRECTORY} 
        ${FOAMEXT_OPENMPI_DIR}
        NO_DEFAULT_PATH
      )
      SET( _foundlib ${${_libname}_OFLIB} )
      IF( _foundlib STREQUAL ${_libname}_OFLIB-NOTFOUND )
        MESSAGE( 
          WARNING 
          "Cannot find ${_libname}. Is it spelled correctly? Correct "
           "capitalization? Do you have another package with similarly-named "
           "libraries, installed at ${FOAMEXT_LINK_DIRECTORY}? (That is "
            "where this script thinks the OF libs are.)" 
        )
      ELSE ( _foundlib STREQUAL ${_libname}_OFLIB-NOTFOUND )
        SET( FOAMEXT_LIBRARIES ${FOAMEXT_LIBRARIES} ${_foundlib} )
      ENDIF( _foundlib STREQUAL ${_libname}_OFLIB-NOTFOUND )
    ENDFOREACH( _libname ${FOAMEXT_FIND_COMPONENTS} )

    #
    # define implementation of scalar and prevent template adding
    # see: http://www.cfd-online.com/Forums/openfoam-programming-development/90676-norepository.html
    #
    ADD_DEFINITIONS( -DWM_DP -DNoRepository)
  ELSE( DEFINED FOAMEXT_FIND_COMPONENTS )
    MESSAGE( 
      AUTHOR_WARNING 
      "Developer must specify required libraries to link against in the cmake "
      "file, i.e. find_package( FOAMEXT REQUIRED COMPONENTS TKernel TKBRep). "
      "Otherwise no libs will be added - linking against ALL OF libraries is "
      "slow!"
    )
  ENDIF( DEFINED FOAMEXT_FIND_COMPONENTS )
ENDIF( FOAMEXT_FOUND STREQUAL TRUE )