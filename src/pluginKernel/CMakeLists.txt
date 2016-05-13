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
  volVectorPatchFieldRange.h
  volScalarPatchFieldRange.h
  analyzeDraftTube.h
  volScalarInChannelFieldRange.h
  volVectorInChannelFieldRange.h
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
  volVectorPatchFieldRange.cpp
  volScalarPatchFieldRange.cpp
  analyzeDraftTube.cpp
  volScalarInChannelFieldRange.cpp
  volVectorInChannelFieldRange.cpp
)

APPEND_DTOO_SOURCE(src/pluginKernel "${SOURCES}")
APPEND_DTOO_HEADER(src/pluginKernel "${HEADERS}")