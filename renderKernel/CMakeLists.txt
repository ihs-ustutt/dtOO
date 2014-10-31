SET(HEADERS
  discrete2dPoints.h
  discrete3dPoints.h
  solid2dLine.h
  solid3dLine.h
  solid3dSurface.h
  unstructured3dMesh.h
)
SET(SOURCES
  discrete2dPoints.cpp
  discrete3dPoints.cpp
  solid2dLine.cpp
  solid3dLine.cpp
  solid3dSurface.cpp
  unstructured3dMesh.cpp
)

APPEND_DTOO_SOURCE(renderKernel "${SOURCES}")
APPEND_DTOO_HEADER(renderKernel "${HEADERS}")