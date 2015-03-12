IF( NOT OpenMesh_FOUND STREQUAL TRUE )
  set( _incsearchpath $ENV{EXTERNLIBS}/OpenMesh/include /usr/include/OpenMesh $ENV{CASROOT}/inc )
  set( _testlibname libOpenMeshCore.so )
  set( _testinc OpenMesh/Core/Mesh/BaseKernel.hh )
  set( _libsearchpath $ENV{EXTERNLIBS}/OpenMesh/lib /usr/lib )

  #find the include dir by looking for Standard_Real.hxx
  FIND_PATH( OpenMesh_INCLUDE_DIR OpenMesh/Core/Mesh/BaseKernel.hh PATHS ${_incsearchpath} DOC "Path to OpenMesh includes" )
  IF( OpenMesh_INCLUDE_DIR STREQUAL ${_testinc}-NOTFOUND )
    SET( OpenMesh_FOUND FALSE CACHE BOOL FORCE)
    MESSAGE( WARNING "Cannot find OpenMesh include dir." )
  ENDIF( OpenMesh_INCLUDE_DIR STREQUAL ${_testinc}-NOTFOUND )

  # Find one lib and save its directory to OpenMesh_LINK_DIRECTORY. Because
  # OM has so many libs, there is increased risk of a name collision.
  # Requiring that all libs be in the same directory reduces the risk.
#  SET(OpenMesh_LINK_DIRECTORY "")
  FIND_PATH( OpenMesh_LINK_DIRECTORY ${_testlibname} PATH ${_libsearchpath} DOC "Path to OpenMesh libs" )
  IF( OpenMesh_LINK_DIRECTORY STREQUAL ${_testlibname}-NOTFOUND )
    SET( OpenMesh_FOUND FALSE CACHE BOOL FORCE)
    MESSAGE( WARNING "Cannot find OpenMesh lib dir." )
  ELSE( OpenMesh_LINK_DIRECTORY STREQUAL ${_testlibname}-NOTFOUND )
    SET( OpenMesh_FOUND TRUE CACHE BOOL "Has OpenMesh been found?" FORCE )
    # CACHE BOOL "Has OM been found?" FORCE )
    #SET( _firsttime TRUE ) #so that messages are only printed once
    MESSAGE( STATUS "Found OpenMesh include dir: ${OpenMesh_INCLUDE_DIR}" )
    MESSAGE( STATUS "Found OpenMesh lib dir: ${OpenMesh_LINK_DIRECTORY}" )
  ENDIF( OpenMesh_LINK_DIRECTORY STREQUAL ${_testlibname}-NOTFOUND )
ENDIF( NOT OpenMesh_FOUND STREQUAL TRUE )

IF( OpenMesh_FOUND STREQUAL TRUE )
  IF( DEFINED OpenMesh_FIND_COMPONENTS )
    FOREACH( _libname ${OpenMesh_FIND_COMPONENTS} )
      #look for libs in OpenMesh_LINK_DIRECTORY
      FIND_LIBRARY( ${_libname}_OpenMeshLIB ${_libname} ${OpenMesh_LINK_DIRECTORY} NO_DEFAULT_PATH)
      SET( _foundlib ${${_libname}_OpenMeshLIB} )
      IF( _foundlib STREQUAL ${_libname}_OpenMeshLIB-NOTFOUND )
        MESSAGE( ERROR "Cannot find ${_libname}. Is it spelled correctly? Correct capitalization? Do you have another package with similarly-named libraries, installed at ${OpenMesh_LINK_DIRECTORY}? (That is where this script thinks the OM libs are.)" )
        SET(OpenMesh_FOUND FALSE CACHE BOOL FORCE)
      ENDIF( _foundlib STREQUAL ${_libname}_OpenMeshLIB-NOTFOUND )
      SET( OpenMesh_LIBRARIES ${OpenMesh_LIBRARIES} ${_foundlib} CACHE TYPE STRING FORCE)
    ENDFOREACH( _libname ${OpenMesh_FIND_COMPONENTS} )
  ELSE( DEFINED OpenMesh_FIND_COMPONENTS )
    MESSAGE( AUTHOR_WARNING "Developer must specify required libraries to link against in the cmake file, i.e. find_package( OpenMesh REQUIRED COMPONENTS lib1 lib2) . Otherwise no libs will be added - linking against ALL OpenMesh libraries is slow!")
  ENDIF( DEFINED OpenMesh_FIND_COMPONENTS )
ENDIF( OpenMesh_FOUND STREQUAL TRUE )
