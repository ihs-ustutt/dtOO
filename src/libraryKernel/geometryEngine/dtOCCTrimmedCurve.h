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

#ifndef DTOCCTRIMMEDCURVE_H
#define DTOCCTRIMMEDCURVE_H

#include <dtOOTypeDef.h>

#include "dtOCCCurve.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

class Geom_TrimmedCurve;

namespace dtOO {
class dtOCCCurveBase;

class dtOCCTrimmedCurve : public dtOCCCurve {
public:
  dt__class(dtOCCTrimmedCurve, dtCurve);
  dtOCCTrimmedCurve();
  dtOCCTrimmedCurve(dtOCCCurveBase const &orig);
  virtual ~dtOCCTrimmedCurve();
  virtual dtOCCTrimmedCurve *clone(void) const;
  virtual dtInt order(void) const;
  virtual dtInt nControlPoints(void) const;
  virtual dtPoint3 controlPoint(dtInt const nPoint) const;
  virtual void setControlPoint(dtInt const nPoint, dtPoint3 const point);

private:
  Geom_TrimmedCurve const *_ptr;
};
} // namespace dtOO

#endif /* DTOCCTRIMMEDCURVE_H */
