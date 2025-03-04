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

#ifndef dtOCCGeomFillBoundWithSurf_H
#define dtOCCGeomFillBoundWithSurf_H

#include <dtOOTypeDef.h>

#include "dtOCCGeomFillBoundary.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

class GeomFill_BoundWithSurf;

namespace dtOO {
class dtOCCCurveBase;

class dtOCCGeomFillBoundWithSurf : public dtOCCGeomFillBoundary {
public:
  dt__class(dtOCCGeomFillBoundWithSurf, dtCurve);
  dtOCCGeomFillBoundWithSurf();
  dtOCCGeomFillBoundWithSurf(
    dtOCCCurveBase const &cBase, dtOCCGeomFillBoundaryBase const &bBase
  );
  virtual ~dtOCCGeomFillBoundWithSurf();
  virtual dtOCCGeomFillBoundWithSurf *clone(void) const;
  virtual dtReal minPara(dtInt const &dir) const;
  virtual dtReal maxPara(dtInt const &dir) const;
  virtual dtPoint3 point(dtReal const uu) const;

private:
  GeomFill_BoundWithSurf const *_ptr;
};
} // namespace dtOO
#endif /* dtOCCGeomFillBoundWithSurf_H */
