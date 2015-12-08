SET(HEADERS
  dtCase.h
  dtCaseFactory.h
  OpenFOAMCase/OpenFOAMCase.h
  OpenFOAMCase/openFOAMSetupRule.h
  OpenFOAMCase/openFOAMWallRule.h
  OpenFOAMCase/openFOAMGgiRule.h
  OpenFOAMCase/openFOAMCyclicGgiRule.h
  OpenFOAMCase/openFOAMEmptyRule.h
  OpenFOAMCase/openFOAMCylindricalInletRule.h
  OpenFOAMCase/openFOAMCellZoneRule.h
)

SET(SOURCES
  dtCase.cpp
  dtCaseFactory.cpp
  OpenFOAMCase/OpenFOAMCase.cpp
  OpenFOAMCase/openFOAMSetupRule.cpp
  OpenFOAMCase/openFOAMWallRule.cpp
  OpenFOAMCase/openFOAMGgiRule.cpp
  OpenFOAMCase/openFOAMCyclicGgiRule.cpp
  OpenFOAMCase/openFOAMEmptyRule.cpp
  OpenFOAMCase/openFOAMCylindricalInletRule.cpp
  OpenFOAMCase/openFOAMCellZoneRule.cpp
)

APPEND_DTOO_SOURCE(solverKernel "${SOURCES}")
APPEND_DTOO_HEADER(solverKernel "${HEADERS}")