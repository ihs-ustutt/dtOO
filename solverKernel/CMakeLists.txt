SET(HEADERS
  dtCase.h
  dtCaseFactory.h
  openFOAM.h
)

SET(SOURCES
  dtCase.cpp
  dtCaseFactory.cpp
  openFOAM.cpp
)

APPEND_DTOO_SOURCE(solverKernel "${SOURCES}")
APPEND_DTOO_HEADER(solverKernel "${HEADERS}")