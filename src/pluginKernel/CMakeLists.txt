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
  volVectorPatchFieldRange.h
  volScalarPatchFieldRange.h
  analyzeDraftTube.h
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
  volVectorPatchFieldRange.cpp
  volScalarPatchFieldRange.cpp
  analyzeDraftTube.cpp
)

APPEND_DTOO_SOURCE(src/pluginKernel "${SOURCES}")
APPEND_DTOO_HEADER(src/pluginKernel "${HEADERS}")