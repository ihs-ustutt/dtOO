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

#ifndef trans4SidedFace_H
#define trans4SidedFace_H

#include <dtOOTypeDef.h>

#include "map2dTo3d.h"
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class map1dTo3d;
class dtTransformer;

class trans4SidedFace : public map2dTo3d {
public:
  dt__class(trans4SidedFace, analyticGeometry);
  trans4SidedFace();
  trans4SidedFace(const trans4SidedFace &orig);
  trans4SidedFace(
    map1dTo3d const *const c0,
    map1dTo3d const *const c1,
    map1dTo3d const *const c2,
    map1dTo3d const *const c3
  );
  virtual trans4SidedFace *create(void) const;
  virtual trans4SidedFace *clone(void) const;
  virtual trans4SidedFace *cloneTransformed(dtTransformer const *const dtT
  ) const;
  virtual ~trans4SidedFace();
  virtual bool isClosed(dtInt const &dir) const;
  virtual dtReal getMin(dtInt const &dir) const;
  virtual dtReal getMax(dtInt const &dir) const;
  virtual dtPoint3 getPoint(dtReal const &uu, dtReal const &vv) const;

private:
  ptrVectorHandling<map1dTo3d> _curves;
  std::vector<dtPoint3> _points;
};
} // namespace dtOO
#endif /* trans4SidedFace_H */
