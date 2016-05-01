SET(HEADERS
  dtCase.h
  dtCaseFactory.h
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

SET(SOURCES
  dtCase.cpp
  dtCaseFactory.cpp
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

APPEND_DTOO_SOURCE(src/solverKernel "${SOURCES}")
APPEND_DTOO_HEADER(src/solverKernel "${HEADERS}")