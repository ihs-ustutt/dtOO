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

#ifndef dtOVMMesh_H
#define dtOVMMesh_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

class MVertex;
class MElement;

#include "dtOVMTypedef.h"

namespace dtOO {
class dtOVMMesh : public ovmMesh {
public:
  dt__classOnlyName(dtOVMMesh);
  dtOVMMesh(int const &nV = 0, dtInt const &nE = 0);
  virtual ~dtOVMMesh();
  //
  // add new vertices and elements
  //
  ovmCellH addCell(::MElement *me);
  //
  // access
  //
  ::MVertex *operator[](ovmVertexH const &vH);
  ovmVertexH const &at(::MVertex const *const mv) const;
  ::MVertex const *const at(ovmVertexH const &vH) const;
  ::MElement *operator[](ovmCellH const &cH);
  ::MElement const *const at(ovmCellH const &cH) const;
  //
  // conversion and replace positions
  //
  void replacePosition(ovmVertexH const &vH, dtPoint3 const &pp);
  //
  // misc
  //
  void makePartition(dtInt const &num) const;
  std::vector<dtPoint3> adjacentVertices(ovmVertexH const &vH) const;

private:
  ovmVertexH addVertex(::MVertex *mv);
  ovmHalffaceH addFace(::MVertex *mv0, ::MVertex *mv1, ::MVertex *mv2);
  ovmHalffaceH addFace(std::vector<::MVertex *> const &mv);

private:
  std::vector<MElement *> _me;
  std::vector<MVertex *> _mv;
  std::map<::MVertex const *, ovmVertexH> _ovm_gmsh;
  std::map<::MElement const *, ovmCellH> _ovm_gmshElement;
};
} // namespace dtOO
#endif /* dtOVMMesh_H */
