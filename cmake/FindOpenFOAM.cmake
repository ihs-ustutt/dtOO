# - Try to find foam-ext libraries
### Does not test what version has been found,though
### that could be done by parsing Standard_Version.hxx


# Once done, this will define
# OpenFOAM_FOUND - true if OF has been found
# OpenFOAM_INCLUDE_DIR - the OF include dir
# OpenFOAM_LIBRARIES - names of OF libraries
# OpenFOAM_LINK_DIRECTORY - location of OF libraries

# Check if foam-ext is sourced and not OpenFOAM
IF ( "$ENV{WM_FORK}" STREQUAL "" )
  # ${OpenFOAM_FOUND} is cached, so once OF is found this block shouldn't have to run again
  IF( NOT OpenFOAM_FOUND STREQUAL TRUE )
    set( _incsearchpath $ENV{FOAM_SRC})
    set( _testlibname libODE.so )
    set( _libsearchpath $ENV{FOAM_LIBBIN} )
    #find the include dir by looking for Standard_Real.hxx
    FIND_PATH( 
      OpenFOAM_INCLUDE_DIR 
      OpenFOAM 
      PATHS ${_incsearchpath} 
      DOC "Path to OF includes" 
      NO_DEFAULT_PATH
    )

    # Find one lib and save its directory to OpenFOAM_LINK_DIRECTORY. Because
    # OF has so many libs, there is increased risk of a name collision.
    # Requiring that all libs be in the same directory reduces the risk.
    FIND_PATH( 
      OpenFOAM_LINK_DIRECTORY 
      ${_testlibname} 
      PATH 
      ${_libsearchpath} 
      DOC 
      "Path to OF libs" 
    )
    file( 
      GLOB OpenFOAM_OPENMPI_DIR
      "${OpenFOAM_LINK_DIRECTORY}/*openmpi*"
    )
    IF( OpenFOAM_LINK_DIRECTORY MATCHES ".*NOTFOUND.*" )
      SET( OpenFOAM_FOUND FALSE CACHE BOOL "Has OF been found?" FORCE )
      MESSAGE( WARNING "Cannot find OF lib dir. Install foam-ext." )
    ELSE( OpenFOAM_LINK_DIRECTORY MATCHES ".*NOTFOUND.*" )
      SET( OpenFOAM_FOUND TRUE CACHE BOOL "Has OF been found?" FORCE )
      MESSAGE( STATUS "Found OF include dir: ${OpenFOAM_INCLUDE_DIR}" )
      MESSAGE( STATUS "Found OF lib dir: ${OpenFOAM_LINK_DIRECTORY}" )
    ENDIF( OpenFOAM_LINK_DIRECTORY MATCHES ".*NOTFOUND.*" )
  ENDIF( NOT OpenFOAM_FOUND STREQUAL TRUE )

  IF( OpenFOAM_FOUND STREQUAL TRUE )
    IF( DEFINED OpenFOAM_FIND_COMPONENTS )
      FOREACH( _libname ${OpenFOAM_FIND_COMPONENTS} )
        #look for libs in OpenFOAM_LINK_DIRECTORY
        FIND_LIBRARY( 
          ${_libname}_OFLIB 
          ${_libname} 
          ${OpenFOAM_LINK_DIRECTORY} 
          ${OpenFOAM_LINK_DIRECTORY}/dummy
          ${OpenFOAM_OPENMPI_DIR}
          NO_DEFAULT_PATH
        )
        SET( _foundlib ${${_libname}_OFLIB} )
        IF( _foundlib STREQUAL ${_libname}_OFLIB-NOTFOUND )
          MESSAGE( 
            WARNING 
            "Cannot find ${_libname}. Is it spelled correctly? Correct "
             "capitalization? Do you have another package with similarly-named "
             "libraries, installed at ${OpenFOAM_LINK_DIRECTORY}? (That is "
             "where this script thinks the OF libs are.)" 
          )
        ELSE ( _foundlib STREQUAL ${_libname}_OFLIB-NOTFOUND )
          SET( OpenFOAM_LIBRARIES ${OpenFOAM_LIBRARIES} ${_foundlib} )
        ENDIF( _foundlib STREQUAL ${_libname}_OFLIB-NOTFOUND )
      ENDFOREACH( _libname ${OpenFOAM_FIND_COMPONENTS} )

      #
      # define implementation of scalar and prevent template adding
      # see: http://www.cfd-online.com/Forums/openfoam-programming-development/90676-norepository.html
      #
      ADD_DEFINITIONS( -DWM_DP -DNoRepository)
    ELSE( DEFINED OpenFOAM_FIND_COMPONENTS )
      MESSAGE( 
        AUTHOR_WARNING 
        "Developer must specify required libraries to link against in the cmake "
        "file, i.e. find_package( FOAMEXT REQUIRED COMPONENTS TKernel TKBRep). "
        "Otherwise no libs will be added - linking against ALL OF libraries is "
        "slow!"
      )
    ENDIF( DEFINED OpenFOAM_FIND_COMPONENTS )
  ENDIF( OpenFOAM_FOUND STREQUAL TRUE )
ELSE( "$ENV{WM_FORK}" STREQUAL "" )
  MESSAGE(
    "**> No OpenFOAM Version initialized. Environment variable WM_FORK is "
    "not empty.\n"
    "**>   WM_FORK=$ENV{WM_FORK}"
  )
ENDIF( "$ENV{WM_FORK}" STREQUAL "" )
