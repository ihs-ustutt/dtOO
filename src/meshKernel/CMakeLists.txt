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
  bVObserver/bVOInterface.h
  bVObserver/bVOSubject.h
  bVObserver/bVOInterfaceFactory.h
  bVObserver/bVOSetGrading.h
  bVObserver/bVONameFaces.h
  bVObserver/bVOSetGradingToFaceRule.h
  bVObserver/bVONameRegions.h
  bVObserver/bVORemoveRegions.h
  bVObserver/bVORemoveElements.h
  bVObserver/bVOWriteMSH.h
  bVObserver/bVOReadMSH.h
  bVObserver/bVOTransfiniteFaces.h
  bVObserver/bVOSetNElements.h
  bVObserver/bVOTransfiniteRegions.h
  bVObserver/bVOOrientCellVolumes.h
  bVObserver/bVORecombineRecursiveRegions.h
  bVObserver/bVOMeshRule.h
  bVObserver/bVOPMeshRule.h
  bVObserver/bVOFaceToPatchRule.h
  bVObserver/bVOAnalyticGeometryToFace.h
  bVObserver/bVORecombine.h
  bVObserver/bVOWriteSTL.h
  bVObserver/bVOSetPrescribedMeshSizeAtPoints.h
  bVObserver/bVODumpModel.h
  bVObserver/bVOEvilGodfather.h
  bVObserver/bVOAddInternalEdge.h
  bVObserver/bVOAddFace.h
  bVObserver/bVOSetRotationalPeriodicity.h
  bVObserver/bVOSetTranslationalPeriodicity.h
  bVObserver/bVOTransformMeshPoints.h
  bVObserver/bVOCoherenceCheck.h
  bVObserver/bVOWriteSU2.h
  bVObserver/bVOSetPrescribedElementSize.h
  bVObserver/bVOForceMeshPointCoordinates.h
  bVObserver/bVOAnalyticFunctionToFace.h
  bVObserver/bVOWriteINP.h
  bVObserver/bVOSetOrder.h
  boundedVolume.h
  boundedVolumeFactory.h
  map3dTo3dGmsh.h
  customGmsh.h
  map2dTo3dTriangulated.h
  map3dTo3dWithInternalGmsh.h
  map3dTo3dWithInternalBlockGmsh.h
  readMOABMesh.h
  gmshBoundedVolume.h
  combineGmsh.h
)

SET(SOURCES
  bVObserver/bVOInterface.cpp
  bVObserver/bVOSubject.cpp
  bVObserver/bVOInterfaceFactory.cpp
  bVObserver/bVOSetGrading.cpp
  bVObserver/bVONameFaces.cpp
  bVObserver/bVOSetGradingToFaceRule.cpp
  bVObserver/bVONameRegions.cpp
  bVObserver/bVORemoveRegions.cpp
  bVObserver/bVORemoveElements.cpp
  bVObserver/bVOWriteMSH.cpp
  bVObserver/bVOReadMSH.cpp
  bVObserver/bVOTransfiniteFaces.cpp
  bVObserver/bVOSetNElements.cpp
  bVObserver/bVOTransfiniteRegions.cpp
  bVObserver/bVOOrientCellVolumes.cpp
  bVObserver/bVORecombineRecursiveRegions.cpp
  bVObserver/bVOMeshRule.cpp
  bVObserver/bVOPMeshRule.cpp
  bVObserver/bVOFaceToPatchRule.cpp
  bVObserver/bVOAnalyticGeometryToFace.cpp
  bVObserver/bVORecombine.cpp
  bVObserver/bVOWriteSTL.cpp
  bVObserver/bVOSetPrescribedMeshSizeAtPoints.cpp
  bVObserver/bVODumpModel.cpp
  bVObserver/bVOEvilGodfather.cpp
  bVObserver/bVOAddInternalEdge.cpp
  bVObserver/bVOAddFace.cpp
  bVObserver/bVOSetRotationalPeriodicity.cpp
  bVObserver/bVOSetTranslationalPeriodicity.cpp
  bVObserver/bVOTransformMeshPoints.cpp
  bVObserver/bVOCoherenceCheck.cpp
  bVObserver/bVOSetPrescribedElementSize.cpp
  bVObserver/bVOWriteSU2.cpp
  bVObserver/bVOForceMeshPointCoordinates.cpp
  bVObserver/bVOAnalyticFunctionToFace.cpp
  bVObserver/bVOWriteINP.cpp
  bVObserver/bVOSetOrder.cpp
  boundedVolume.cpp
  boundedVolumeFactory.cpp
  map3dTo3dGmsh.cpp
  customGmsh.cpp
  map2dTo3dTriangulated.cpp
  map3dTo3dWithInternalGmsh.cpp
  map3dTo3dWithInternalBlockGmsh.cpp
  readMOABMesh.cpp
  gmshBoundedVolume.cpp
  combineGmsh.cpp
)

APPEND_DTOO_SOURCE(src/meshKernel "${SOURCES}")
APPEND_DTOO_HEADER(src/meshKernel "${HEADERS}")
