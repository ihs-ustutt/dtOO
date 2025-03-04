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

#ifndef multipleBoundedVolume_H
#define multipleBoundedVolume_H

#include <dtOOTypeDef.h>

#include "analyticGeometry.h"
#include <dtLinearAlgebra.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class multipleBoundedVolume : public analyticGeometry {
public:
  dt__class(multipleBoundedVolume, analyticGeometry);
  multipleBoundedVolume();
  multipleBoundedVolume(const multipleBoundedVolume &orig);
  virtual ~multipleBoundedVolume();
  multipleBoundedVolume(
    analyticGeometry const *const m3d,
    ptrVectorHandling<analyticGeometry> const &m2d
  );

  virtual multipleBoundedVolume *create(void) const;
  virtual multipleBoundedVolume *clone(void) const;
  virtual dtInt dim(void) const;
  virtual bool isClosed(dtInt const &dir) const;
  virtual dtReal getMin(dtInt const &dir) const;
  virtual dtReal getMax(dtInt const &dir) const;
  virtual dtPoint3 getPoint(dtReal const *const uvw) const;
  vectorHandling<renderInterface *> getRender(void) const;
  ptrVectorHandling<analyticGeometry> const &boundsVectorConstRef(void) const;
  analyticGeometry const *const regionConstPtr(void) const;

private:
  ptrHandling<analyticGeometry> _m3d;
  ptrVectorHandling<analyticGeometry> _m2d;
};
} // namespace dtOO
#endif /* multipleBoundedVolume_H */
