IF( NOT COVISE_FOUND STREQUAL TRUE )
  set( _incsearchpath $ENV{COVISEDIR}/src/kernel )
  set( _testlibname libcoApi.so )
#  set( _testinc COVISE/Core/Mesh/BaseKernel.hh )
  set( _libsearchpath $ENV{COVISEDIR}/$ENV{ARCHSUFFIX}/lib )

  #find the include dir by looking for Standard_Real.hxx
  FIND_PATH( COVISE_INCLUDE_DIR api/coModule.h PATHS ${_incsearchpath} DOC "Path to COVISE includes" )
  IF( COVISE_INCLUDE_DIR STREQUAL ${_testinc}-NOTFOUND )
    SET( COVISE_FOUND FALSE CACHE BOOL FORCE)
    MESSAGE( WARNING "Cannot find COVISE include dir." )
  ENDIF( COVISE_INCLUDE_DIR STREQUAL ${_testinc}-NOTFOUND )

  # Find one lib and save its directory to COVISE_LINK_DIRECTORY. Because
  # OM has so many libs, there is increased risk of a name collision.
  # Requiring that all libs be in the same directory reduces the risk.
#  SET(COVISE_LINK_DIRECTORY "")
  FIND_PATH( COVISE_LINK_DIRECTORY ${_testlibname} PATH ${_libsearchpath} DOC "Path to COVISE libs" )
  IF( COVISE_LINK_DIRECTORY STREQUAL ${_testlibname}-NOTFOUND )
    SET( COVISE_FOUND FALSE CACHE BOOL FORCE)
    MESSAGE( WARNING "Cannot find COVISE lib dir." )
  ELSE( COVISE_LINK_DIRECTORY STREQUAL ${_testlibname}-NOTFOUND )
    SET( COVISE_FOUND TRUE CACHE BOOL "Has COVISE been found?" FORCE )
    # CACHE BOOL "Has OM been found?" FORCE )
    #SET( _firsttime TRUE ) #so that messages are only printed once
    MESSAGE( STATUS "Found COVISE include dir: ${COVISE_INCLUDE_DIR}" )
    MESSAGE( STATUS "Found COVISE lib dir: ${COVISE_LINK_DIRECTORY}" )
  ENDIF( COVISE_LINK_DIRECTORY STREQUAL ${_testlibname}-NOTFOUND )
ENDIF( NOT COVISE_FOUND STREQUAL TRUE )

IF( COVISE_FOUND STREQUAL TRUE )
  IF( DEFINED COVISE_FIND_COMPONENTS )
    FOREACH( _libname ${COVISE_FIND_COMPONENTS} )
      #look for libs in COVISE_LINK_DIRECTORY
      FIND_LIBRARY( ${_libname}_COVISELIB ${_libname} ${COVISE_LINK_DIRECTORY} NO_DEFAULT_PATH)
      SET( _foundlib ${${_libname}_COVISELIB} )
      IF( _foundlib STREQUAL ${_libname}_COVISELIB-NOTFOUND )
        MESSAGE( ERROR "Cannot find ${_libname}. Is it spelled correctly? Correct capitalization? Do you have another package with similarly-named libraries, installed at ${COVISE_LINK_DIRECTORY}? (That is where this script thinks the OM libs are.)" )
        SET(COVISE_FOUND FALSE CACHE BOOL FORCE)
      ENDIF( _foundlib STREQUAL ${_libname}_COVISELIB-NOTFOUND )
      SET( COVISE_LIBRARIES ${COVISE_LIBRARIES} ${_foundlib} CACHE TYPE STRING FORCE)
    ENDFOREACH( _libname ${COVISE_FIND_COMPONENTS} )
  ELSE( DEFINED COVISE_FIND_COMPONENTS )
    MESSAGE( AUTHOR_WARNING "Developer must specify required libraries to link against in the cmake file, i.e. find_package( COVISE REQUIRED COMPONENTS lib1 lib2) . Otherwise no libs will be added - linking against ALL COVISE libraries is slow!")
  ENDIF( DEFINED COVISE_FIND_COMPONENTS )
ENDIF( COVISE_FOUND STREQUAL TRUE )
