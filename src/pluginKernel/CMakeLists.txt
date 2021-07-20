SET(HEADERS
  dtPlugin.h
  dtPluginFactory.h
  dtPluginDriver.h
  dtPluginKernel.h
  writeStep.h
  constValueAssingRule.h
  analyticFunctionToCSV.h
  analyticGeometryToCSV.h
)

SET(SOURCES
  dtPlugin.cpp
  dtPluginFactory.cpp
  dtPluginDriver.cpp
  dtPluginKernel.cpp
  writeStep.cpp
  constValueAssingRule.cpp
  analyticFunctionToCSV.cpp
  analyticGeometryToCSV.cpp
)

if (FOAMEXT_FOUND)
  list(APPEND HEADERS
    volVectorFieldVersusXYZ.h
    volScalarOnLineFieldRange.h
    pOnBlade.h
    UcylInChannel.h
    uRelInChannel.h
    meshQuality.h
    volVectorPatchFieldRange.h
    volScalarPatchFieldRange.h
    volScalarInChannelFieldRange.h
    volVectorInChannelFieldRange.h
    volVectorFieldRange.h
    volScalarFieldRange.h
    volVectorOnRotatingLineFieldRange.h
  )

  list(APPEND SOURCES
    volVectorFieldVersusXYZ.cpp
    volScalarOnLineFieldRange.cpp
    pOnBlade.cpp
    UcylInChannel.cpp
    uRelInChannel.cpp
    meshQuality.cpp
    volVectorPatchFieldRange.cpp
    volScalarPatchFieldRange.cpp
    volScalarInChannelFieldRange.cpp
    volVectorInChannelFieldRange.cpp
    volVectorFieldRange.cpp
    volScalarFieldRange.cpp
    volVectorOnRotatingLineFieldRange.cpp
  )
else (FOAMEXT_FOUND)
  message("**> Do not include foam-ext based plugins")
endif (FOAMEXT_FOUND)

APPEND_DTOO_SOURCE(src/pluginKernel "${SOURCES}")
APPEND_DTOO_HEADER(src/pluginKernel "${HEADERS}")