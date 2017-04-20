#ifndef dtOVMTypedef_H
#define dtOVMTypedef_H

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

