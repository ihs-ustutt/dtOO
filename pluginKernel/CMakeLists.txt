SET(HEADERS
  dtPlugin.h
  dtPluginFactory.h
  dtPluginDriver.h
  writeStep.h
  constValueAssingRule.h
  createOpenFOAMCase/createOpenFOAMCase.h
  createOpenFOAMCase/openFOAMSetupRule.h
  createOpenFOAMCase/openFOAMWallRule.h
  createOpenFOAMCase/openFOAMGgiRule.h
)

SET(SOURCES
  dtPlugin.cpp
  dtPluginFactory.cpp
  dtPluginDriver.cpp
  writeStep.cpp
  constValueAssingRule.cpp
  createOpenFOAMCase/createOpenFOAMCase.cpp
  createOpenFOAMCase/openFOAMSetupRule.cpp
  createOpenFOAMCase/openFOAMWallRule.cpp
  createOpenFOAMCase/openFOAMGgiRule.cpp
)

APPEND_DTOO_SOURCE(pluginKernel "${SOURCES}")
APPEND_DTOO_HEADER(pluginKernel "${HEADERS}")