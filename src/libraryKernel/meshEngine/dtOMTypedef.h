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

#ifndef dtOMTypedef_H
#define dtOMTypedef_H

#include <dtOOTypeDef.h>

#include "dtOMMeshTraits.h"

namespace dtOO {
//
// handles
//
typedef ::OpenMesh::VertexHandle omVertexH;
typedef ::OpenMesh::HalfedgeHandle omHalfedgeH;
typedef ::OpenMesh::EdgeHandle omEdgeH;
typedef ::OpenMesh::FaceHandle omFaceH;
//
// data
//
typedef omMesh::VertexData omVertexD;
typedef omMesh::HalfedgeData omHalfedgeD;
typedef omMesh::EdgeData omEdgeD;
typedef omMesh::FaceData omFaceD;
//
// iterators
//
typedef omMesh::VertexIter omVertexI;
typedef omMesh::ConstVertexIter omConstVertexI;
typedef omMesh::VertexVertexIter omVertexVertexI;
typedef omMesh::ConstVertexVertexIter omConstVertexVertexI;
typedef omMesh::FaceIter omFaceI;
typedef omMesh::ConstFaceIter omConstFaceI;
typedef omMesh::HalfedgeIter omHalfedgeI;
typedef omMesh::ConstHalfedgeIter omConstHalfedgeI;
typedef omMesh::EdgeIter omEdgeI;
typedef omMesh::ConstEdgeIter omConstEdgeI;
typedef omMesh::VertexFaceIter omVertexFaceI;
typedef omMesh::FaceFaceIter omFaceFaceI;
typedef omMesh::ConstFaceFaceIter omConstFaceFaceI;
typedef omMesh::ConstVertexFaceIter omConstVertexFaceI;
typedef omMesh::FaceVertexIter omFaceVertexI;
typedef omMesh::ConstFaceVertexIter omConstFaceVertexI;
typedef omMesh::VertexEdgeIter omVertexEdgeI;
typedef omMesh::ConstVertexEdgeIter omConstVertexEdgeI;
typedef omMesh::VertexOHalfedgeIter omVertexOHalfedgeI;
typedef omMesh::ConstVertexOHalfedgeIter omConstVertexOHalfedgeI;
typedef omMesh::VertexIHalfedgeIter omVertexIHalfedgeI;
typedef omMesh::ConstVertexIHalfedgeIter omConstVertexIHalfedgeI;
typedef omMesh::FaceEdgeIter omFaceEdgeI;
typedef omMesh::ConstFaceEdgeIter omConstFaceEdgeI;
typedef omMesh::FaceHalfedgeIter omFaceHalfedgeI;
typedef omMesh::ConstFaceHalfedgeIter omConstFaceHalfedgeI;

//
// misc
//
typedef omMesh::Point omPoint;
typedef omMesh::Scalar omScalar;
typedef omMesh::Normal omNormal;
} // namespace dtOO
#endif /* dtOMTypedef_H */
