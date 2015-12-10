SET(HEADERS
  dtPlugin.h
  dtPluginFactory.h
  dtPluginDriver.h
  dtPluginKernel.h
  writeStep.h
  constValueAssingRule.h
  analyticFunctionToCSV.h
  volVectorFieldVersusRZ.h
  volScalarFieldVersusL.h
  pOnBlade.h
)

SET(SOURCES
  dtPlugin.cpp
  dtPluginFactory.cpp
  dtPluginDriver.cpp
  dtPluginKernel.cpp
  writeStep.cpp
  constValueAssingRule.cpp
  analyticFunctionToCSV.cpp
  volVectorFieldVersusRZ.cpp
  volScalarFieldVersusL.cpp
  pOnBlade.cpp
)

APPEND_DTOO_SOURCE(pluginKernel "${SOURCES}")
APPEND_DTOO_HEADER(pluginKernel "${HEADERS}")