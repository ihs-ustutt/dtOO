IF( OpenCASCADE_FOUND STREQUAL TRUE )
  RETURN()
ENDIF()
#IF( NOT OpenCASCADE_FOUND STREQUAL TRUE )
  #
  # config search paths
  #
  set( 
    _configsearchpath 
    $ENV{CASROOT}/lib/cmake/opencascade
    $ENV{CASROOT}/lib64/cmake/opencascade
    $ENV{DTOO_EXTERNLIBS}/lib/cmake/opencascade
    $ENV{DTOO_EXTERNLIBS}/lib64/cmake/opencascade
    /usr/lib/cmake/opencascade                                         
    /usr/lib64/cmake/opencascade                                       
    /usr/lib/x86_64-linux-gnu/cmake/opencascade
  )

  #
  # find path to config
  #
  find_path(
    OpenCASCADE_CONFIG_DIR
    OpenCASCADEConfig.cmake PATHS
    ${_configsearchpath}
    DOC
    "Path to config"
  )

  # return if not found
  if(OpenCASCADE_CONFIG_DIR MATCHES "^.*NOTFOUND.*$")
    SET( OpenCASCADE_FOUND FALSE CACHE BOOL FORCE )
    MESSAGE( 
      FATAL_ERROR 
      "Cannot find OpenCASCADE config file. Install opencascade or set CASROOT."
    )
    RETURN()
  endif()

  #
  # set variables if OCC is found
  #
  SET( OpenCASCADE_FOUND TRUE CACHE BOOL "Has OCC been found?" FORCE )
  SET( OpenCASCADE_CONFIG_FILE "${OpenCASCADE_CONFIG_DIR}/OpenCASCADEConfig.cmake" )
  # include config file
  INCLUDE("${OpenCASCADE_CONFIG_FILE}")
  SET(OpenCASCADE_LINK_DIRECTORY "${OpenCASCADE_LIBRARY_DIR}")
  # reset variable
  SET(OpenCASCADE_LIBRARIES "")
  SET( 
    OpenCASCADE_VERSION_STRING 
    "${OpenCASCADE_MAJOR_VERSION}.${OpenCASCADE_MINOR_VERSION}.${OpenCASCADE_MAINTENANCE_VERSION}"
  )

  #
  # necessary libraries
  #
  SET( OpenCASCADE_FIND_LIBRARIES "")
  LIST(
    APPEND OpenCASCADE_FIND_LIBRARIES  
    TKernel TKMath
    TKFillet TKShHealing 
    TKTopAlgo TKBRep 
    TKGeomBase TKGeomAlgo 
    TKG3d TKG2d
    TKXSBase
  )

  #
  # version dependent library names
  #
  if(OpenCASCADE_VERSION_STRING VERSION_LESS 7.8.0)
    LIST(APPEND OpenCASCADE_FIND_LIBRARIES TKSTEP TKIGES)
  else()
    LIST(APPEND OpenCASCADE_FIND_LIBRARIES TKDESTEP TKDEIGES)
  endif() 

  #
  # search necessary libraries
  #
  FOREACH( _libname ${OpenCASCADE_FIND_LIBRARIES} )
    FIND_LIBRARY( 
      ${_libname}_OCCLIB ${_libname} 
      ${OpenCASCADE_LINK_DIRECTORY} NO_DEFAULT_PATH
    )
    SET( _foundlib ${${_libname}_OCCLIB} )
    IF( _foundlib STREQUAL ${_libname}_OCCLIB-NOTFOUND )
      MESSAGE( FATAL_ERROR "Cannot find ${_libname}.")
    ENDIF( _foundlib STREQUAL ${_libname}_OCCLIB-NOTFOUND )
    SET( OpenCASCADE_LIBRARIES ${OpenCASCADE_LIBRARIES} ${_foundlib} )
  ENDFOREACH()

  #
  # output
  #
  MESSAGE( 
   STATUS 
   "|\n"
   "| OpenCASCADE_FOUND=${OpenCASCADE_FOUND}\n"
   "| OpenCASCADE_INCLUDE_DIR}=${OpenCASCADE_INCLUDE_DIR}\n"
   "| OpenCASCADE_LINK_DIRECTORY=${OpenCASCADE_LINK_DIRECTORY}\n"
   "|   Version : ${OpenCASCADE_MAJOR_VERSION}.${OpenCASCADE_MINOR_VERSION}.${OpenCASCADE_MAINTENANCE_VERSION}\n"
   "|   Version : ${OpenCASCADE_VERSION_STRING}\n"
   "|   Libraries : ${OpenCASCADE_LIBRARIES}\n"
   "|" 
   )
 #ENDIF()
