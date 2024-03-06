/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#ifndef dtOVMTypedef_H
#define dtOVMTypedef_H

#include <dtOOTypeDef.h>

#include <OpenVolumeMesh/Mesh/PolyhedralMesh.hh>
#include <OpenVolumeMesh/Core/OpenVolumeMeshProperty.hh>

namespace dtOO {
  //
  // typedefs
  //
  typedef ::OpenVolumeMesh::GeometricPolyhedralMeshV3f ovmMesh;
  //
  // handles
  //
  typedef ::OpenVolumeMesh::VertexHandle ovmVertexH;
  typedef ::OpenVolumeMesh::HalfEdgeHandle ovmHalfedgeH;
  typedef ::OpenVolumeMesh::EdgeHandle ovmEdgeH;
  typedef ::OpenVolumeMesh::HalfFaceHandle ovmHalffaceH;
  typedef ::OpenVolumeMesh::FaceHandle ovmFaceH;
  typedef ::OpenVolumeMesh::CellHandle ovmCellH;
  //
  // iterators
  //
  typedef ::OpenVolumeMesh::VertexIter ovmVertexI;
  typedef ::OpenVolumeMesh::VertexOHalfEdgeIter ovmVertexOHalfedgeI;
  typedef ::OpenVolumeMesh::VertexCellIter ovmVertexCellI;
  typedef ::OpenVolumeMesh::CellVertexIter ovmCellVertexI;
  //
  // misc
  //
  typedef ::OpenVolumeMesh::Vec3f ovmPoint;
}
#endif /* dtOVMTypedef_H */
