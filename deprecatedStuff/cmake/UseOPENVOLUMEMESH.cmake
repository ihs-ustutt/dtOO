MACRO(USE_OPENVOLUMEMESH)
  SET(OPENVOLUMEMESH_FIND_REQUIRED FALSE)
  FIND_PACKAGE(OPENVOLUMEMESH COMPONENTS OpenVolumeMesh)
  IF ((NOT OPENVOLUMEMESH_FOUND))
    MESSAGE("Skipping because of missing OpenVolumeMesh")
    RETURN()
  ENDIF((NOT OPENVOLUMEMESH_FOUND))
  IF(NOT OPENVOLUMEMESH_USED AND OPENVOLUMEMESH_FOUND)
    SET(OPENVOLUMEMESH_USED TRUE)
    INCLUDE_DIRECTORIES(${OPENVOLUMEMESH_INCLUDE_DIR})
    LINK_DIRECTORIES(${OPENVOLUMEMESH_LINK_DIRECTORY})
    SET(EXTRA_LIBS ${EXTRA_LIBS} ${OPENVOLUMEMESH_LIBRARIES})
    #
    # OpenVolumeMesh: prevents error when linking
    #    
    ADD_DEFINITIONS(-DINCLUDE_TEMPLATES)
  ENDIF()
ENDMACRO(USE_OPENVOLUMEMESH)