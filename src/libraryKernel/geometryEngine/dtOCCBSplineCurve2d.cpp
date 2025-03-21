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

#include "dtOCCBSplineCurve2d.h"

#include "dtOCCCurve2dBase.h"
#include "progHelper.h"
#include <logMe/logMe.h>

#include <Geom2d_BSplineCurve.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_Failure.hxx>
#include <Standard_TypeDef.hxx>
#include <gp_Pnt2d.hxx>

namespace dtOO {
dtOCCBSplineCurve2d::dtOCCBSplineCurve2d() : dtOCCCurve2d() { _ptr = NULL; }

dtOCCBSplineCurve2d::dtOCCBSplineCurve2d(dtOCCCurve2dBase const &orig)
  : dtOCCCurve2d(orig)
{
  dt__mustCast(OCCRef().getOCC().get(), Geom2d_BSplineCurve const, _ptr);
}

dtOCCBSplineCurve2d::~dtOCCBSplineCurve2d() {}

dtOCCBSplineCurve2d *dtOCCBSplineCurve2d::clone(void) const
{
  return new dtOCCBSplineCurve2d(OCCRef());
}

dtInt dtOCCBSplineCurve2d::order(void) const
{
  return static_cast<int>(_ptr->Degree());
}

dtInt dtOCCBSplineCurve2d::nControlPoints(void) const
{
  return static_cast<int>(_ptr->NbPoles());
}

dtPoint2 dtOCCBSplineCurve2d::controlPoint(dtInt const nPoint) const
{
  Standard_Integer nPI = static_cast<Standard_Integer>(nPoint + 1);
  gp_Pnt2d pp = _ptr->Pole(nPI);

  return dtPoint2(
    static_cast<dtReal>(pp.Coord(1)), static_cast<dtReal>(pp.Coord(2))
  );
}

void dtOCCBSplineCurve2d::setControlPoint(
  dtInt const nPoint, dtPoint2 const point
)
{
  Standard_Integer nPI = static_cast<Standard_Integer>(nPoint + 1);
  gp_Pnt2d pp(
    static_cast<Standard_Real>(point.x()), static_cast<Standard_Real>(point.y())
  );

  Handle(Geom2d_Curve) occC = dtOCCCurve2d::OCCRef().getOCC();
  Handle(Geom2d_BSplineCurve) occBsc =
    Handle(Geom2d_BSplineCurve)::DownCast(occC);

  dt__tryOcc(occBsc->SetPole(nPI, pp);,
                                      << "SetPole fails." << std::endl
                                      << "nPoint = " << nPoint << std::endl
                                      << "pp = " << dt__point2d(point)
                                      << std::endl);
}
} // namespace dtOO
