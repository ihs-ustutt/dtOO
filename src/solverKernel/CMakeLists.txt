set(HEADERS
  dtCase.h
  dtCaseFactory.h
)

set(SOURCES
  dtCase.cpp
  dtCaseFactory.cpp
)

if (OpenFOAM_FOUND)
  list(APPEND HEADERS
    OpenFOAMCase/OpenFOAMCase.h
    OpenFOAMCase/OpenFOAMSetupRule.h
    OpenFOAMCase/OpenFOAMWallRule.h
    OpenFOAMCase/OpenFOAMGgiRule.h
    OpenFOAMCase/OpenFOAMCyclicGgiRule.h
    OpenFOAMCase/OpenFOAMMixingPlaneRule.h
    OpenFOAMCase/OpenFOAMEmptyRule.h
    OpenFOAMCase/OpenFOAMCylindricalInletRule.h
    OpenFOAMCase/OpenFOAMCellZoneRule.h
  )

  list(APPEND SOURCES
    OpenFOAMCase/OpenFOAMCase.cpp
    OpenFOAMCase/OpenFOAMSetupRule.cpp
    OpenFOAMCase/OpenFOAMWallRule.cpp
    OpenFOAMCase/OpenFOAMGgiRule.cpp
    OpenFOAMCase/OpenFOAMCyclicGgiRule.cpp
    OpenFOAMCase/OpenFOAMMixingPlaneRule.cpp
    OpenFOAMCase/OpenFOAMEmptyRule.cpp
    OpenFOAMCase/OpenFOAMCylindricalInletRule.cpp
    OpenFOAMCase/OpenFOAMCellZoneRule.cpp
  )
else (OpenFOAM_FOUND)
  message("**> Do not include foam-ext based cases")
endif (OpenFOAM_FOUND)

APPEND_DTOO_SOURCE(src/solverKernel "${SOURCES}")
APPEND_DTOO_HEADER(src/solverKernel "${HEADERS}")