IF( NOT OPENVOLUMEMESH_FOUND STREQUAL TRUE )
  set( _incsearchpath $ENV{EXTERNLIBS}/OpenVolumeMesh/include /usr/include/OpenVolumeMesh $ENV{CASROOT}/inc )
  set( _testlibname libOpenVolumeMesh.so )
  set( _testinc OpenVolumeMesh/Core/GeometryKernel.hh )
  set( _libsearchpath $ENV{EXTERNLIBS}/OpenVolumeMesh/lib/OpenVolumeMesh/ /usr/lib $ENV{CASROOT}/lib )

  #find the include dir by looking for Standard_Real.hxx
  FIND_PATH( OPENVOLUMEMESH_INCLUDE_DIR ${_testinc} PATHS ${_incsearchpath} DOC "Path to OpenVolumeMesh includes" )
  IF( OPENVOLUMEMESH_INCLUDE_DIR STREQUAL ${_testinc}-NOTFOUND )
    SET( OPENVOLUMEMESH_FOUND FALSE CACHE BOOL FORCE)
    MESSAGE( WARNING "Cannot find OpenVolumeMesh include dir." )
  ENDIF( OPENVOLUMEMESH_INCLUDE_DIR STREQUAL ${_testinc}-NOTFOUND )

  # Find one lib and save its directory to OPENVOLUMEMESH_LINK_DIRECTORY. Because
  # OM has so many libs, there is increased risk of a name collision.
  # Requiring that all libs be in the same directory reduces the risk.
#  SET(OPENVOLUMEMESH_LINK_DIRECTORY "")
  FIND_PATH( OPENVOLUMEMESH_LINK_DIRECTORY ${_testlibname} PATH ${_libsearchpath} DOC "Path to OpenVolumeMesh libs" )
  IF( OPENVOLUMEMESH_LINK_DIRECTORY STREQUAL ${_testlibname}-NOTFOUND )
    SET( OPENVOLUMEMESH_FOUND FALSE CACHE BOOL FORCE)
    MESSAGE( WARNING "Cannot find OpenVolumeMesh lib dir." )
  ELSE( OPENVOLUMEMESH_LINK_DIRECTORY STREQUAL ${_testlibname}-NOTFOUND )
    SET( OPENVOLUMEMESH_FOUND TRUE CACHE BOOL "Has OpenVolumeMesh been found?" FORCE )
    # CACHE BOOL "Has OM been found?" FORCE )
    #SET( _firsttime TRUE ) #so that messages are only printed once
    MESSAGE( STATUS "Found OpenVolumeMesh include dir: ${OPENVOLUMEMESH_INCLUDE_DIR}" )
    MESSAGE( STATUS "Found OpenVolumeMesh lib dir: ${OPENVOLUMEMESH_LINK_DIRECTORY}" )
  ENDIF( OPENVOLUMEMESH_LINK_DIRECTORY STREQUAL ${_testlibname}-NOTFOUND )
ENDIF( NOT OPENVOLUMEMESH_FOUND STREQUAL TRUE )

IF( OPENVOLUMEMESH_FOUND STREQUAL TRUE )
  IF( DEFINED OPENVOLUMEMESH_FIND_COMPONENTS )
    FOREACH( _libname ${OPENVOLUMEMESH_FIND_COMPONENTS} )
      #look for libs in OPENVOLUMEMESH_LINK_DIRECTORY
      FIND_LIBRARY( ${_libname}_OpenVolumeMeshLIB ${_libname} ${OPENVOLUMEMESH_LINK_DIRECTORY} NO_DEFAULT_PATH)
      SET( _foundlib ${${_libname}_OpenVolumeMeshLIB} )
      IF( _foundlib STREQUAL ${_libname}_OpenVolumeMeshLIB-NOTFOUND )
        MESSAGE( ERROR "Cannot find ${_libname}. Is it spelled correctly? Correct capitalization? Do you have another package with similarly-named libraries, installed at ${OPENVOLUMEMESH_LINK_DIRECTORY}? (That is where this script thinks the OM libs are.)" )
        SET(OPENVOLUMEMESH_FOUND FALSE CACHE BOOL FORCE)
      ENDIF( _foundlib STREQUAL ${_libname}_OpenVolumeMeshLIB-NOTFOUND )
      SET( OPENVOLUMEMESH_LIBRARIES ${OPENVOLUMEMESH_LIBRARIES} ${_foundlib})# CACHE TYPE STRING FORCE)
    ENDFOREACH( _libname ${OPENVOLUMEMESH_FIND_COMPONENTS} )
  ELSE( DEFINED OPENVOLUMEMESH_FIND_COMPONENTS )
    MESSAGE( AUTHOR_WARNING "Developer must specify required libraries to link against in the cmake file, i.e. find_package( OpenVolumeMesh REQUIRED COMPONENTS lib1 lib2) . Otherwise no libs will be added - linking against ALL OpenVolumeMesh libraries is slow!")
  ENDIF( DEFINED OPENVOLUMEMESH_FIND_COMPONENTS )
ENDIF( OPENVOLUMEMESH_FOUND STREQUAL TRUE )
