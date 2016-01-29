SET(HEADERS
  dtPlugin.h
  dtPluginFactory.h
  dtPluginDriver.h
  dtPluginKernel.h
  writeStep.h
  constValueAssingRule.h
  analyticFunctionToCSV.h
  volVectorFieldVersusXYZ.h
  volScalarFieldVersusL.h
  pOnBlade.h
  UcylInChannel.h
  uRelInChannel.h
  meshQuality.h
  volScalarFieldRange.h
)

SET(SOURCES
  dtPlugin.cpp
  dtPluginFactory.cpp
  dtPluginDriver.cpp
  dtPluginKernel.cpp
  writeStep.cpp
  constValueAssingRule.cpp
  analyticFunctionToCSV.cpp
  volVectorFieldVersusXYZ.cpp
  volScalarFieldVersusL.cpp
  pOnBlade.cpp
  UcylInChannel.cpp
  uRelInChannel.cpp
  meshQuality.cpp
  volScalarFieldRange.cpp
)

APPEND_DTOO_SOURCE(pluginKernel "${SOURCES}")
APPEND_DTOO_HEADER(pluginKernel "${HEADERS}")