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

#include "dtOCCLine2d.h"

#include "dtOCCCurve2dBase.h"
#include "progHelper.h"
#include <logMe/logMe.h>

#include <Standard_TypeDef.hxx>
// #include <gp_Pnt2d.hxx>
#include <Geom2d_Line.hxx>

namespace dtOO {
dtOCCLine2d::dtOCCLine2d() : dtOCCCurve2d() { _ptr = NULL; }

dtOCCLine2d::dtOCCLine2d(dtOCCCurve2dBase const &orig) : dtOCCCurve2d(orig)
{
  dt__mustCast(OCCRef().getOCC().get(), Geom2d_Line const, _ptr);
}

dtOCCLine2d::~dtOCCLine2d() {}

dtOCCLine2d *dtOCCLine2d::clone(void) const
{
  return new dtOCCLine2d(OCCRef());
}

dtInt dtOCCLine2d::order(void) const { dt__throwUnexpected(order()); }

dtInt dtOCCLine2d::nControlPoints(void) const { return 0; }

dtPoint2 dtOCCLine2d::controlPoint(dtInt const nPoint) const
{
  dt__throwUnexpected(controlPoint());
}

} // namespace dtOO
