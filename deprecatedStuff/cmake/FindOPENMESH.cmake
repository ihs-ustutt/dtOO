IF( NOT OPENMESH_FOUND STREQUAL TRUE )
  set( _incsearchpath $ENV{EXTERNLIBS}/openmesh/include /usr/include/openmesh $ENV{CASROOT}/inc )
  set( _testlibname libOpenMeshCored.so )
  set( _testinc OpenMesh/Core/Mesh/BaseKernel.hh )
  set( _libsearchpath $ENV{EXTERNLIBS}/openmesh/lib/OpenMesh/ /usr/lib $ENV{CASROOT}/lib )

  #find the include dir by looking for Standard_Real.hxx
  FIND_PATH( OPENMESH_INCLUDE_DIR OpenMesh/Core/Mesh/BaseKernel.hh PATHS ${_incsearchpath} DOC "Path to OpenMesh includes" )
  IF( OPENMESH_INCLUDE_DIR STREQUAL ${_testinc}-NOTFOUND )
    SET( OPENMESH_FOUND FALSE CACHE BOOL FORCE)
    MESSAGE( WARNING "Cannot find OpenMesh include dir." )
  ENDIF( OPENMESH_INCLUDE_DIR STREQUAL ${_testinc}-NOTFOUND )

  # Find one lib and save its directory to OPENMESH_LINK_DIRECTORY. Because
  # OM has so many libs, there is increased risk of a name collision.
  # Requiring that all libs be in the same directory reduces the risk.
#  SET(OPENMESH_LINK_DIRECTORY "")
  FIND_PATH( OPENMESH_LINK_DIRECTORY ${_testlibname} PATH ${_libsearchpath} DOC "Path to OpenMesh libs" )
  IF( OPENMESH_LINK_DIRECTORY STREQUAL ${_testlibname}-NOTFOUND )
    SET( OPENMESH_FOUND FALSE CACHE BOOL FORCE)
    MESSAGE( WARNING "Cannot find OpenMesh lib dir." )
  ELSE( OPENMESH_LINK_DIRECTORY STREQUAL ${_testlibname}-NOTFOUND )
    SET( OPENMESH_FOUND TRUE CACHE BOOL "Has OpenMesh been found?" FORCE )
    # CACHE BOOL "Has OM been found?" FORCE )
    #SET( _firsttime TRUE ) #so that messages are only printed once
    MESSAGE( STATUS "Found OpenMesh include dir: ${OPENMESH_INCLUDE_DIR}" )
    MESSAGE( STATUS "Found OpenMesh lib dir: ${OPENMESH_LINK_DIRECTORY}" )
  ENDIF( OPENMESH_LINK_DIRECTORY STREQUAL ${_testlibname}-NOTFOUND )
ENDIF( NOT OPENMESH_FOUND STREQUAL TRUE )

IF( OPENMESH_FOUND STREQUAL TRUE )
  IF( DEFINED OPENMESH_FIND_COMPONENTS )
    FOREACH( _libname ${OPENMESH_FIND_COMPONENTS} )
      #look for libs in OPENMESH_LINK_DIRECTORY
      FIND_LIBRARY( ${_libname}_OPENMESHLIB ${_libname} ${OPENMESH_LINK_DIRECTORY} NO_DEFAULT_PATH)
      SET( _foundlib ${${_libname}_OPENMESHLIB} )
      IF( _foundlib STREQUAL ${_libname}_OPENMESHLIB-NOTFOUND )
        MESSAGE( ERROR "Cannot find ${_libname}. Is it spelled correctly? Correct capitalization? Do you have another package with similarly-named libraries, installed at ${OPENMESH_LINK_DIRECTORY}? (That is where this script thinks the OM libs are.)" )
        SET(OPENMESH_FOUND FALSE CACHE BOOL FORCE)
      ENDIF( _foundlib STREQUAL ${_libname}_OPENMESHLIB-NOTFOUND )
      SET( OPENMESH_LIBRARIES ${OPENMESH_LIBRARIES} ${_foundlib})# CACHE TYPE STRING FORCE)
    ENDFOREACH( _libname ${OPENMESH_FIND_COMPONENTS} )
  ELSE( DEFINED OPENMESH_FIND_COMPONENTS )
    MESSAGE( AUTHOR_WARNING "Developer must specify required libraries to link against in the cmake file, i.e. find_package( OPENMESH REQUIRED COMPONENTS lib1 lib2) . Otherwise no libs will be added - linking against ALL OpenMesh libraries is slow!")
  ENDIF( DEFINED OPENMESH_FIND_COMPONENTS )
ENDIF( OPENMESH_FOUND STREQUAL TRUE )
