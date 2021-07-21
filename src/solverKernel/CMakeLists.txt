set(HEADERS
  dtCase.h
  dtCaseFactory.h
)

set(SOURCES
  dtCase.cpp
  dtCaseFactory.cpp
)

if (FOAMEXT_FOUND)
  list(APPEND HEADERS
    feOpenFOAMCase/feOpenFOAMCase.h
    feOpenFOAMCase/feOpenFOAMSetupRule.h
    feOpenFOAMCase/feOpenFOAMWallRule.h
    feOpenFOAMCase/feOpenFOAMGgiRule.h
    feOpenFOAMCase/feOpenFOAMCyclicGgiRule.h
    feOpenFOAMCase/feOpenFOAMMixingPlaneRule.h
    feOpenFOAMCase/feOpenFOAMEmptyRule.h
    feOpenFOAMCase/feOpenFOAMCylindricalInletRule.h
    feOpenFOAMCase/feOpenFOAMCellZoneRule.h
  )

  list(APPEND SOURCES
    feOpenFOAMCase/feOpenFOAMCase.cpp
    feOpenFOAMCase/feOpenFOAMSetupRule.cpp
    feOpenFOAMCase/feOpenFOAMWallRule.cpp
    feOpenFOAMCase/feOpenFOAMGgiRule.cpp
    feOpenFOAMCase/feOpenFOAMCyclicGgiRule.cpp
    feOpenFOAMCase/feOpenFOAMMixingPlaneRule.cpp
    feOpenFOAMCase/feOpenFOAMEmptyRule.cpp
    feOpenFOAMCase/feOpenFOAMCylindricalInletRule.cpp
    feOpenFOAMCase/feOpenFOAMCellZoneRule.cpp
  )
  if (OpenFoamTurbo_FOUND)
    list(APPEND HEADERS
      feOpenFOAMCase/feOpenFOAMTurboRule.h
    )

    list(APPEND SOURCES
      feOpenFOAMCase/feOpenFOAMTurboRule.cpp
    )
  else (OpenFoamTurbo_FOUND)
    message("**> Do not include OpenFoamTurbo based cases")
  endif (OpenFoamTurbo_FOUND)
else (FOAMEXT_FOUND)
  message("**> Do not include foam-ext based cases")
endif (FOAMEXT_FOUND)

APPEND_DTOO_SOURCE(src/solverKernel "${SOURCES}")
APPEND_DTOO_HEADER(src/solverKernel "${HEADERS}")