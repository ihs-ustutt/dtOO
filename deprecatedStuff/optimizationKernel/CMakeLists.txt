SET(HEADERS
  optimizableInterface.h
  eoLib.h
  eoLibEval.h
  characterizable.h
)
SET(SOURCES
  optimizableInterface.cpp
  eoLib.cpp
  characterizable.cpp
)

APPEND_DTOO_SOURCE(optimizationKernel "${SOURCES}")
APPEND_DTOO_HEADER(optimizationKernel "${HEADERS}")

#INCLUDE_DIRECTORIES(../geometryKernel ../generalKernel ../libraryKernel ../meshKernel)