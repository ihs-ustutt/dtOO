SET(HEADERS
  bVObserver/bVOInterface.h
  bVObserver/bVOSubject.h
  bVObserver/bVOInterfaceFactory.h
  bVObserver/bVOSetNElements.h
  bVObserver/bVOSetGrading.h
  bVObserver/bVOSetSimpleGrading.h
  boundedVolume.h
  boundedVolumeFactory.h
  map3dTo3dBlockGmsh.h
  map3dTo3dTriangulated.h
  map3dTo3dWithInternalTurboGrid.h
  vec3dInMap3dTo3dWithMeshedSurface.h
)

SET(SOURCES
  bVObserver/bVOInterface.cpp
  bVObserver/bVOSubject.cpp
  bVObserver/bVOInterfaceFactory.cpp
  bVObserver/bVOSetNElements.cpp
  bVObserver/bVOSetGrading.cpp
  bVObserver/bVOSetSimpleGrading.cpp
  boundedVolume.cpp
  boundedVolumeFactory.cpp
  map3dTo3dBlockGmsh.cpp
  map3dTo3dTriangulated.cpp
  map3dTo3dWithInternalTurboGrid.cpp
  vec3dInMap3dTo3dWithMeshedSurface.cpp
)

APPEND_DTOO_SOURCE(meshKernel "${SOURCES}")
APPEND_DTOO_HEADER(meshKernel "${HEADERS}")