MACRO(USE_OpenCASCADE)
  SET(OpenCASCADE_FIND_REQUIRED FALSE) 
  FIND_PACKAGE(OpenCASCADE COMPONENTS TKernel TKSTEP TKIGES TKXSBase TKOffset 
                                      TKFeat TKFillet TKBool TKShHealing TKMesh 
                                      TKHLR TKBO TKPrim TKTopAlgo TKBRep 
                                      TKGeomAlgo TKGeomBase TKG3d TKG2d 
                                      TKAdvTools TKMath TKernel TKSTEPAttr 
                                      TKSTEP209 TKSTEPBase)
  IF ((NOT OpenCASCADE_FOUND))
    USING_MESSAGE("Skipping because of missing OpenCASCADE")
    RETURN()
  ENDIF((NOT OpenCASCADE_FOUND))
  IF(NOT OpenCASCADE_USED AND OpenCASCADE_FOUND)
    SET(OpenCASCADE_USED TRUE)
    INCLUDE_DIRECTORIES(${OpenCASCADE_INCLUDE_DIR})
    SET(EXTRA_LIBS ${EXTRA_LIBS} ${OpenCASCADE_LIBRARIES})
  ENDIF()
ENDMACRO(USE_OpenCASCADE)