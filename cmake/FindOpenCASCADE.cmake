# - Try to find OpenCASCADE libraries
### Does not test what version has been found,though
### that could be done by parsing Standard_Version.hxx


# Once done, this will define
# OpenCASCADE_FOUND - true if OCC has been found
# OpenCASCADE_INCLUDE_DIR - the OCC include dir
# OpenCASCADE_LIBRARIES - names of OCC libraries
# OpenCASCADE_LINK_DIRECTORY - location of OCC libraries

# ${OpenCASCADE_FOUND} is cached, so once OCC is found this block shouldn't have to run again
IF( NOT OpenCASCADE_FOUND STREQUAL TRUE )
  set( _testlibname libTKernel.so )
  set( 
    _incsearchpath 
    $ENV{DTOO_EXTERNLIBS}/include
    $ENV{DTOO_EXTERNLIBS}/include/oce
    $ENV{DTOO_EXTERNLIBS}/include/opencascade
    /usr/include  
    /usr/include/oce
    /usr/include/opencascade
  )
  set( 
    _libsearchpath 
    $ENV{DTOO_EXTERNLIBS}/lib
    $ENV{DTOO_EXTERNLIBS}/lib64
    $ENV{DTOO_EXTERNLIBS}/lib/oce
    $ENV{DTOO_EXTERNLIBS}/lib64/oce
    /usr/lib                                         
    /usr/lib64                                       
    /usr/lib/oce
    /usr/lib64/oce
    /usr/lib/x86_64-linux-gnu
  )

  #find the include dir by looking for Standard_Real.hxx
  FIND_PATH( 
    OpenCASCADE_INCLUDE_DIR 
    Standard_Real.hxx PATHS 
    ${_incsearchpath} 
    DOC 
    "Path to OCC includes" 
  )

  IF( OpenCASCADE_INCLUDE_DIR STREQUAL Standard_Real.hxx-NOTFOUND )
    SET( OpenCASCADE_FOUND FALSE CACHE BOOL FORCE )
    MESSAGE( 
      FATAL_ERROR 
      "Cannot find OCC include dir. Install opencascade or set CASROOT or \
       create a symlink /opt/occ/inc pointing to the correct directory." 
    )
  ENDIF( OpenCASCADE_INCLUDE_DIR STREQUAL Standard_Real.hxx-NOTFOUND )

  # Find one lib and save its directory to OpenCASCADE_LINK_DIRECTORY. Because
  # OCC has so many libs, there is increased risk of a name collision.
  # Requiring that all libs be in the same directory reduces the risk.
  FIND_PATH( 
    OpenCASCADE_LINK_DIRECTORY 
    ${_testlibname} 
    PATH 
    ${_libsearchpath} 
    DOC 
    "Path to OCC libs" 
  )

  IF( OpenCASCADE_LINK_DIRECTORY STREQUAL ${_testlibname}-NOTFOUND )
    SET( OpenCASCADE_FOUND FALSE CACHE BOOL FORCE )
    MESSAGE( 
      FATAL_ERROR 
      "Cannot find OCC lib dir. Install opencascade or set CASROOT or \
       create a symlink /opt/occ/lib pointing to the dir where the OCC \
       libs are."
    )
  ELSE( OpenCASCADE_LINK_DIRECTORY STREQUAL ${_testlibname}-NOTFOUND )
    SET( OpenCASCADE_FOUND TRUE CACHE BOOL "Has OCC been found?" FORCE )
    SET( _firsttime TRUE ) #so that messages are only printed once
    MESSAGE( STATUS "Found OCC include dir: ${OpenCASCADE_INCLUDE_DIR}" )
    MESSAGE( STATUS "Found OCC lib dir: ${OpenCASCADE_LINK_DIRECTORY}" )
  ENDIF( OpenCASCADE_LINK_DIRECTORY STREQUAL ${_testlibname}-NOTFOUND )
ELSE( NOT OpenCASCADE_FOUND STREQUAL TRUE )
  SET( _firsttime FALSE ) #so that messages are only printed once
ENDIF( NOT OpenCASCADE_FOUND STREQUAL TRUE )

IF( OpenCASCADE_FOUND STREQUAL TRUE )
  IF( DEFINED OpenCASCADE_FIND_COMPONENTS )
    FOREACH( _libname ${OpenCASCADE_FIND_COMPONENTS} )
      #look for libs in OpenCASCADE_LINK_DIRECTORY
      FIND_LIBRARY( 
        ${_libname}_OCCLIB ${_libname} 
        ${OpenCASCADE_LINK_DIRECTORY} NO_DEFAULT_PATH
      )
      SET( _foundlib ${${_libname}_OCCLIB} )
      IF( _foundlib STREQUAL ${_libname}_OCCLIB-NOTFOUND )
        MESSAGE( 
          FATAL_ERROR 
          "Cannot find ${_libname}. Is it spelled correctly? Correct \
           capitalization? Do you have another package with similarly-named \
           libraries, installed at ${OpenCASCADE_LINK_DIRECTORY}? (That is \
           where this script thinks the OCC libs are.)" 
        )
      ENDIF( _foundlib STREQUAL ${_libname}_OCCLIB-NOTFOUND )
      SET( OpenCASCADE_LIBRARIES ${OpenCASCADE_LIBRARIES} ${_foundlib} )
    ENDFOREACH( _libname ${OpenCASCADE_FIND_COMPONENTS} )

    IF (UNIX)
      ADD_DEFINITIONS( -DLIN -DLININTEL )
    ENDIF (UNIX)

    # 32 bit or 64 bit?
    IF( CMAKE_SIZEOF_VOID_P EQUAL 4 )
      IF( _firsttime STREQUAL TRUE )
        MESSAGE( STATUS "This is a 32-bit system." )
      ENDIF( _firsttime STREQUAL TRUE )
    ELSE( CMAKE_SIZEOF_VOID_P EQUAL 4 )
      IF( _firsttime STREQUAL TRUE )
        MESSAGE( STATUS "This is a 64-bit system. Adding appropriate compiler flags for OCC." )
      ENDIF( _firsttime STREQUAL TRUE )
      ADD_DEFINITIONS( -D_OCC64 )
      IF (UNIX)
        ADD_DEFINITIONS( -m64 )
      ENDIF (UNIX)
    ENDIF( CMAKE_SIZEOF_VOID_P EQUAL 4 )

    ADD_DEFINITIONS( -DHAVE_CONFIG_H -DHAVE_IOSTREAM -DHAVE_FSTREAM -DHAVE_LIMITS_H -DHAVE_IOMANIP )
  ELSE( DEFINED OpenCASCADE_FIND_COMPONENTS )
    MESSAGE( 
      AUTHOR_WARNING 
      "Developer must specify required libraries to link \
       against in the cmake file, i.e. \
       find_package( OpenCASCADE REQUIRED COMPONENTS TKernel TKBRep) . \
       Otherwise no libs will be added - linking against ALL OCC libraries \
       is slow!"
      )
  ENDIF( DEFINED OpenCASCADE_FIND_COMPONENTS )
ENDIF( OpenCASCADE_FOUND STREQUAL TRUE )
