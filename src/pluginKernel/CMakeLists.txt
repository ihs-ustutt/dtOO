#------------------------------------------------------------------------------
#  dtOO < design tool Object-Oriented >
#    
#    Copyright (C) 2024 A. Tismer.
#------------------------------------------------------------------------------
#License
#    This file is part of dtOO.
#
#    dtOO is distributed in the hope that it will be useful, but WITHOUT
#    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
#    dtOO root directory for more details.
#
#    You should have received a copy of the License along with dtOO.
#
#------------------------------------------------------------------------------

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
