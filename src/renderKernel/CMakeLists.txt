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
  discrete2dPoints.h
  discrete3dPoints.h
  solid2dLine.h
  solid3dLine.h
  solid3dSurface.h
  unstructured3dMesh.h
  discrete3dVector.h
  unstructured3dSurfaceMesh.h
)
SET(SOURCES
  discrete2dPoints.cpp
  discrete3dPoints.cpp
  solid2dLine.cpp
  solid3dLine.cpp
  solid3dSurface.cpp
  unstructured3dMesh.cpp
  discrete3dVector.cpp
  unstructured3dSurfaceMesh.cpp
)

APPEND_DTOO_SOURCE(src/renderKernel "${SOURCES}")
APPEND_DTOO_HEADER(src/renderKernel "${HEADERS}")
