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

#ifndef DTOMMESHMANIFOLD_H
#define DTOMMESHMANIFOLD_H

#include <dtOOTypeDef.h>

#include "dtOMMesh.h"
#include <logMe/dtMacros.h>

namespace dtOO {
class dtOMMeshManifold : public dtOMMesh {
public:
  dt__classOnlyName(dtOMMeshManifold);
  dtOMMeshManifold(dtOMMesh const &om, omVertexH const &vH);
  dtOMMeshManifold(dtOMMeshManifold const &orig);
  virtual ~dtOMMeshManifold();
  dtReal minDihedralAngle(void) const;
  dtReal maxDihedralAngle(void) const;
  bool divideable(void) const;
  bool closed(void) const;
  virtual void update(void);
  dtOMMeshManifold divide(void);
  std::vector<dtOMMeshManifold> divide(dtReal const &angle);
  dtVector3 normal(void) const;
  MVertex *centerMVertex(void) const;

private:
  dtOMMeshManifold subractManifold(omEdgeH const &from, omEdgeH const &to);

private:
  std::vector<dtReal> _dihedralAngleV;
  std::vector<bool> _isBoundary;
  omVertexH _centerVertex;
  dtInt _nTri;
  dtInt _nQuad;
};
} // namespace dtOO
#endif /* DTOMMESHMANIFOLD_H */
