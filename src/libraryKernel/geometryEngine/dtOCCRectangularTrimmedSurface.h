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

#ifndef DTOCCRECTANGULARTRIMMEDSURFACE_H
#define DTOCCRECTANGULARTRIMMEDSURFACE_H

#include <dtOOTypeDef.h>

#include "dtOCCSurface.h"
#include <logMe/dtMacros.h>

class Geom_RectangularTrimmedSurface;

namespace dtOO {
class dtOCCRectangularTrimmedSurface : public dtOCCSurface {
public:
  dt__class(dtOCCRectangularTrimmedSurface, dtSurface);
  dtOCCRectangularTrimmedSurface();
  dtOCCRectangularTrimmedSurface(const dtOCCSurfaceBase &orig);
  virtual ~dtOCCRectangularTrimmedSurface();
  virtual dtOCCRectangularTrimmedSurface *clone(void) const;
  virtual dtCurve *
  segmentConstU(dtReal const uu, dtReal const vvMin, dtReal const vvMax) const;
  virtual dtCurve *
  segmentConstV(dtReal const vv, dtReal const uuMin, dtReal const uuMax) const;

private:
  Geom_RectangularTrimmedSurface const *_ptr;
};
} // namespace dtOO
#endif /* DTOCCRECTANGULARTRIMMEDSURFACE_H */
