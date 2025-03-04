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

#ifndef DTOCCBSPLINESURFACE2D_H
#define DTOCCBSPLINESURFACE2D_H

#include <dtOOTypeDef.h>

#include "dtOCCSurface2d.h"
#include <logMe/dtMacros.h>

class Geom_BSplineSurface;

namespace dtOO {
class dtOCCSurfaceBase;

class dtOCCBSplineSurface2d : public dtOCCSurface2d {
public:
  dt__class(dtOCCBSplineSurface2d, dtSurface2d);
  dtOCCBSplineSurface2d();
  dtOCCBSplineSurface2d(const dtOCCSurfaceBase &orig);
  virtual ~dtOCCBSplineSurface2d();
  virtual dtOCCBSplineSurface2d *clone(void) const;
  virtual dtCurve2d *
  segmentConstU(dtReal const uu, dtReal const vvMin, dtReal const vvMax) const;
  virtual dtCurve2d *
  segmentConstV(dtReal const vv, dtReal const uuMin, dtReal const uuMax) const;
  virtual dtPoint2 controlPoint(dtInt const uI, dtInt const vI) const;
  virtual void
  setControlPoint(dtInt const uI, dtInt const vI, dtPoint2 const point);
  virtual dtInt nControlPoints(dtInt const dim) const;

private:
  Geom_BSplineSurface const *_ptr;
};
} // namespace dtOO
#endif /* DTOCCBSPLINESURFACE2D_H */
