SET(HEADERS
  bVObserver/bVOInterface.h
  bVObserver/bVOSubject.h
  bVObserver/bVOInterfaceFactory.h
  bVObserver/bVOSplitEdge.h
  boundedVolume.h
  boundedVolumeFactory.h
  map3dTo3dCompoundVolume.h
  map3dTo3dTriangulated.h
  map3dTo3dWithInternalTurboGrid.h
)

SET(SOURCES
  bVObserver/bVOInterface.cpp
  bVObserver/bVOSubject.cpp
  bVObserver/bVOInterfaceFactory.cpp
  bVObserver/bVOSplitEdge.cpp
  boundedVolume.cpp
  boundedVolumeFactory.cpp
  map3dTo3dCompoundVolume.cpp
  map3dTo3dTriangulated.cpp
  map3dTo3dWithInternalTurboGrid.cpp
)

APPEND_DTOO_SOURCE(meshKernel "${SOURCES}")
APPEND_DTOO_HEADER(meshKernel "${HEADERS}")