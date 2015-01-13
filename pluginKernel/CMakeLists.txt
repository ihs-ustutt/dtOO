SET(HEADERS
  dtPlugin.h
  dtPluginFactory.h
  writeStep.h
)

SET(SOURCES
  dtPlugin.cpp
  dtPluginFactory.cpp
  writeStep.cpp
)

APPEND_DTOO_SOURCE(pluginKernel "${SOURCES}")
APPEND_DTOO_HEADER(pluginKernel "${HEADERS}")