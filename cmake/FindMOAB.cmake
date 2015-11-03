# Once done, this will define
# MOAB_FOUND - true if MOAB has been found
# MOAB_INCLUDE_DIR - the MOAB include dir
# MOAB_LIBRARIES - names of MOAB libraries
# MOAB_LINK_DIRECTORY - location of MOAB libraries

# ${MOAB_FOUND} is cached, so once MOAB is found this block shouldn't have to run again
#IF( NOT MOAB_FOUND STREQUAL TRUE )
#  set( 
#    _incsearchpath 
#    /usr/ihs/include/
#    ${DTOO_EXTERNLIBS}/moab/include/ 
#  )
#  set( 
#    _testlibname libMOAB.so 
#  )
#  set( 
#    _libsearchpath 
#    /usr/ihs/lib/
#    /usr/ihs/lib64/
#    ${DTOO_EXTERNLIBS}/ALL/lib/
#  )
#  #find the include dir by looking for moab/Version.h
#  FIND_PATH( 
#    MOAB_INCLUDE_DIR moab/Version.h 
#    PATHS 
#    ${_incsearchpath}
#    DOC 
#    "Path to MOAB foam includes" 
#  )
#  IF( MOAB_INCLUDE_DIR STREQUAL MOAB_INCLUDE_DIR-NOTFOUND )
#    SET( MOAB_FOUND FALSE CACHE BOOL FORCE )
#    MESSAGE( FATAL_ERROR "Cannot find MOAB include dir. Install MOAB." )
#  ENDIF( MOAB_INCLUDE_DIR STREQUAL MOAB_INCLUDE_DIR-NOTFOUND )
#
#  # Find one lib and save its directory to MOAB_LINK_DIRECTORY. Because
#  # MOAB has so many libs, there is increased risk of a name collision.
#  # Requiring that all libs be in the same directory reduces the risk.
#  FIND_PATH( 
#    MOAB_LINK_DIRECTORY ${_testlibname} 
#    PATH 
#    ${_libsearchpath} 
#    DOC 
#    "Path to MOAB libs" 
#  )
#  IF( MOAB_LINK_DIRECTORY STREQUAL ${_testlibname}-NOTFOUND )
#    SET( MOAB_FOUND FALSE CACHE BOOL FORCE )
#    MESSAGE( FATAL_ERROR "Cannot find MOAB lib dir. Install openfoam." )
#  ELSE( MOAB_LINK_DIRECTORY STREQUAL ${_testlibname}-NOTFOUND )
#    SET( MOAB_FOUND TRUE CACHE BOOL "Has MOAB been found?" FORCE )
#    SET( _firsttime TRUE ) #so that messages are only printed once
#    MESSAGE( 
#      STATUS 
#      "|\n"
#      "| Found MOAB\n"
#      "| MOAB_INCLUDE_DIR=${MOAB_INCLUDE_DIR}\n"
#      "| MOAB_LINK_DIRECTORY=${MOAB_LINK_DIRECTORY}\n"
#      "|" 
#      )
#  ENDIF( MOAB_LINK_DIRECTORY STREQUAL ${_testlibname}-NOTFOUND )
#ELSE( NOT MOAB_FOUND STREQUAL TRUE )
#  SET( _firsttime FALSE ) #so that messages are only printed once
#ENDIF( NOT MOAB_FOUND STREQUAL TRUE )
#
#IF( MOAB_FOUND STREQUAL TRUE )
#  IF( DEFINED MOAB_FIND_COMPONENTS )
#    FOREACH( _libname ${MOAB_FIND_COMPONENTS} )
#      #look for libs in MOAB_LINK_DIRECTORY
#      FIND_LIBRARY( 
#        ${_libname}_MOABLIB 
#        ${_libname} 
#        ${MOAB_LINK_DIRECTORY} 
#        NO_DEFAULT_PATH
#      )
#      SET( _foundlib ${${_libname}_MOABLIB} )
#      IF( _foundlib STREQUAL ${_libname}_MOABLIB-NOTFOUND )
#        MESSAGE( 
#          FATAL_ERROR 
#          "Cannot find ${_libname}. Is it spelled correctly? Correct \
#           capitalization? Do you have another package with similarly-named \
#           libraries, installed at ${MOAB_LINK_DIRECTORY}?" 
#        )
#      ENDIF( _foundlib STREQUAL ${_libname}_MOABLIB-NOTFOUND )
#      SET( MOAB_LIBRARIES ${MOAB_LIBRARIES} ${_foundlib} )
#    ENDFOREACH( _libname ${MOAB_FIND_COMPONENTS} )
#  ELSE( DEFINED MOAB_FIND_COMPONENTS )
#    MESSAGE( 
#     AUTHOR_WARNING 
#     "Developer must specify required libraries to link against in the cmake \
#      file, i.e. find_package( MOAB REQUIRED COMPONENTS lib1 lib2) . \
#      Otherwise no libs will be added - linking against ALL MOAB libraries is \
#      slow!"
#    )
#  ENDIF( DEFINED MOAB_FIND_COMPONENTS )
#ENDIF( MOAB_FOUND STREQUAL TRUE )
DTOO_FIND_LIBRARY(
  "MOAB_FOUND" "moab/Version.h" "libMOAB.so" 
  "MOAB_INCLUDE_DIR" "MOAB_LINK_DIRECTORY" "MOAB_LIBRARIES" 
  "MOAB_FIND_COMPONENTS"
)