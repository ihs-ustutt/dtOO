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

#ifndef DTOCCSURFACEOFREVOLUTION_H
#define DTOCCSURFACEOFREVOLUTION_H

#include <dtOOTypeDef.h>

#include "dtOCCSurface.h"
#include <logMe/dtMacros.h>

class Geom_SurfaceOfRevolution;

namespace dtOO {
class dtOCCSurfaceOfRevolution : public dtOCCSurface {
public:
  dt__class(dtOCCSurfaceOfRevolution, dtSurface);
  dtOCCSurfaceOfRevolution();
  dtOCCSurfaceOfRevolution(const dtOCCSurfaceBase &orig);
  virtual ~dtOCCSurfaceOfRevolution();
  virtual dtOCCSurfaceOfRevolution *clone(void) const;
  virtual dtCurve *
  segmentConstU(dtReal const uu, dtReal const vvMin, dtReal const vvMax) const;
  virtual dtCurve *
  segmentConstV(dtReal const vv, dtReal const uuMin, dtReal const uuMax) const;

private:
  Geom_SurfaceOfRevolution const *_ptr;
};
} // namespace dtOO
#endif /* DTOCCSURFACEOFREVOLUTION_H */
