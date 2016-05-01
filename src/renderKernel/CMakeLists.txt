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