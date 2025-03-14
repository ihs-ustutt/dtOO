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

#ifndef dtOMMesh_H
#define dtOMMesh_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

#include "dtOMMeshTraits.h"
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
namespace dtOO {
typedef OpenMesh::PolyMesh_ArrayKernelT<dtOO::dtOMMeshTraits> omMesh;
}
#include "dtOMTypedef.h"

class MVertex;
class MElement;

namespace dtOO {
class dtOMField;
/**
 * @brief Interface to OpenMesh
 *
 * Manages interface to OpenMesh. Please add new functions of
 * of OpenMesh only in this class.
 * See <a href="http://www.openmesh.org">OpenMesh</a>.
 */
class dtOMMesh : public omMesh {
public:
  dt__classOnlyName(dtOMMesh);
  dtOMMesh();
  dtOMMesh(dtOMMesh const &orig);
  virtual ~dtOMMesh();
  void update(void);
  //
  // add new vertices and elements
  //
  omFaceH addFace(omFaceD const &fD);
  omFaceH addFace(::MElement const *const me);
  omFaceH addFaceInv(::MElement const *const me);
  void add(const dtOMMesh &toAdd);
  void addInv(const dtOMMesh &toAdd);
  //
  // access
  //
  ::MVertex *operator[](omVertexH const &vH);
  omVertexH const &at(::MVertex const *const mv) const;
  ::MVertex const *const at(omVertexH const &vH) const;
  ::MElement *operator[](omFaceH const &fH);
  ::MElement const *const at(omFaceH const &fH) const;
  omFaceH const &at(::MElement const *const me) const;
  //
  // check functions
  //
  bool contains(::MVertex const *const mv) const;
  bool contains(omFaceH const &fH, omEdgeH const &eH) const;
  bool contains(omFaceH const &fH, omVertexH const &vH) const;
  bool intersection(
    std::vector<omFaceH> const &fH,
    dtPoint3 const &start,
    dtPoint3 const &target
  ) const;
  std::vector<omVertexH> oneRingVertexH(omVertexH const &vH) const;
  //
  // conversion and replace positions
  //
  static dtPoint3 toDtPoint3(omPoint const &oP);
  static dtVector3 toDtVector3(omNormal const &nP);
  void replaceMVertex(omVertexH const &vH, ::MVertex *mv);
  void replaceMElement(omFaceH const &fH, ::MElement *me);
  void replacePosition(omVertexH const &vH, dtPoint3 const &pp);
  //
  // attach fields
  //
  void enqueueField(dtOMField *omField);
  void dequeueField(dtOMField *omField);
  dtOMField *operator[](std::string const &fieldName);
  //
  // output
  //
  void writeMesh(std::string const filename) const;
  //      bool vertexIsBoundary(MVertex * mv) const;
  //      bool isGeometricalEdge( omEdgeH const & eH) const;
  //      std::pair< omVertexH const, omVertexH const >
  //      foldVertices( omEdgeH const & eH) const;
  //      std::pair< omFaceH, omFaceH > foldFaces( omEdgeH const & eH) const;
  //
  // input
  //
  void readMesh(std::string const filename);
  //
  // mesh navigation
  //
  omHalfedgeH sameHalfedgeInNextFace(omHalfedgeH const &heH) const;
  dtInt nVertices(omFaceH const &fH) const;
  void createNewElements(void);
  void updateMap(bool vv = true, bool ff = true);

private:
  std::map<::MVertex const *, omVertexH> const &omGmsh(void) const;
  omVertexH addVertex(::MVertex const *const &mv);
  omFaceH addFace(std::vector<::MVertex *> const &vertices);
  omFaceH
  addFace(std::vector<::MVertex *> const &vertices, ::MElement const *const me);
  bool intersection(
    omFaceH const &fH, dtPoint3 const &start, dtPoint3 const &target
  ) const;
  void
  faceVertices(omFaceH const &fH, std::vector<::MVertex *> &vertices) const;

private:
  std::map<::MVertex const *, omVertexH> _om_gmsh;
  std::map<::MElement const *, omFaceH> _om_gmshElement;
  std::vector<dtOMField *> _attachedField;
  dt__pVH(::MElement) _elements;
};
} // namespace dtOO
#endif /* dtOMMesh_H */
