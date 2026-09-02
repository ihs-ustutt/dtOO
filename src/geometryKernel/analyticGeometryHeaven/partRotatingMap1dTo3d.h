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

#ifndef partRotatingMap1dTo3d_H
#define partRotatingMap1dTo3d_H

#include "rotatingMap1dTo3d.h"

namespace dtOO {
class partRotatingMap1dTo3d : public rotatingMap1dTo3d {
public:
  dt__class(partRotatingMap1dTo3d, analyticGeometry);
  partRotatingMap1dTo3d();
  partRotatingMap1dTo3d(
    dtVector3 const &vv,
    map1dTo3d const *const m1d,
    dtReal const &minB,
    dtReal const &maxB
  );
  partRotatingMap1dTo3d(const partRotatingMap1dTo3d &orig);
  virtual ~partRotatingMap1dTo3d();
  virtual partRotatingMap1dTo3d *create(void) const;
  virtual partRotatingMap1dTo3d *clone(void) const;
  virtual partRotatingMap1dTo3d *cloneTransformed(dtTransformer const *const dtT
  ) const;
  virtual bool isClosed(dtInt const &dir) const;
  virtual dtReal getMin(dtInt const &dir) const;
  virtual dtReal getMax(dtInt const &dir) const;
  virtual dtPoint3 getPoint(dtReal const &uu, dtReal const &vv) const;
  virtual map1dTo3d *segmentConstU(dtReal const &uu) const;

private:
  dtReal _minB;
  dtReal _maxB;
};
} // namespace dtOO
#endif /* partRotatingMap1dTo3d_H */
