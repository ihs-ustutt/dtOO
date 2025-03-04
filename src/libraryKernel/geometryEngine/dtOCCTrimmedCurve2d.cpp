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

#include "dtOCCTrimmedCurve2d.h"

#include "dtOCCCurve2dBase.h"
#include "progHelper.h"
#include <logMe/logMe.h>

#include <Standard_TypeDef.hxx>
// #include <gp_Pnt2d.hxx>
#include <Geom2d_TrimmedCurve.hxx>

namespace dtOO {
dtOCCTrimmedCurve2d::dtOCCTrimmedCurve2d() : dtOCCCurve2d() { _ptr = NULL; }

dtOCCTrimmedCurve2d::dtOCCTrimmedCurve2d(
  dtOCCCurve2dBase const &orig, dtReal const u0, dtReal const u1
)
  : dtOCCCurve2d(orig)
{
  dt__mustCast(OCCRef().getOCC().get(), Geom2d_TrimmedCurve const, _ptr);
  _u0 = u0;
  _u1 = u1;
}

dtOCCTrimmedCurve2d::~dtOCCTrimmedCurve2d() {}

dtOCCTrimmedCurve2d *dtOCCTrimmedCurve2d::clone(void) const
{
  return new dtOCCTrimmedCurve2d(OCCRef(), _u0, _u1);
}

dtInt dtOCCTrimmedCurve2d::order(void) const { dt__throwUnexpected(order()); }

dtInt dtOCCTrimmedCurve2d::nControlPoints(void) const { return 0; }

dtPoint2 dtOCCTrimmedCurve2d::controlPoint(dtInt const nPoint) const
{
  dt__throwUnexpected(controlPoint());
}

dtReal dtOCCTrimmedCurve2d::getU0(void) const { return _u0; }

dtReal dtOCCTrimmedCurve2d::getU1(void) const { return _u1; }
} // namespace dtOO
