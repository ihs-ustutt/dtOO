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

#include "dtOCCSurfaceOfRevolution.h"

#include "dtOCCBSplineCurve.h"
#include "dtOCCBezierCurve.h"
#include "dtOCCCurveBase.h"
#include "dtOCCRectangularTrimmedSurface.h"
#include "dtOCCSurfaceBase.h"
#include "dtOCCTrimmedCurve.h"
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

#include <Geom_BSplineCurve.hxx>
#include <Geom_BezierCurve.hxx>
#include <Geom_Conic.hxx>
#include <Geom_RectangularTrimmedSurface.hxx>
#include <Geom_SurfaceOfRevolution.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <Standard_TypeDef.hxx>
#include <gp_Pnt.hxx>

namespace dtOO {
dtOCCSurfaceOfRevolution::dtOCCSurfaceOfRevolution() : dtOCCSurface()
{
  _ptr = NULL;
}

dtOCCSurfaceOfRevolution::dtOCCSurfaceOfRevolution(const dtOCCSurfaceBase &orig)
  : dtOCCSurface(orig)
{
  dt__mustCast(OCCRef().getOCC().get(), Geom_SurfaceOfRevolution const, _ptr);
}

dtOCCSurfaceOfRevolution::~dtOCCSurfaceOfRevolution() {}

dtOCCSurfaceOfRevolution *dtOCCSurfaceOfRevolution::clone(void) const
{
  return new dtOCCSurfaceOfRevolution(OCCRef());
}

dtCurve *dtOCCSurfaceOfRevolution::segmentConstU(
  dtReal const uu, dtReal const vvMin, dtReal const vvMax
) const
{
  Standard_Real uR = static_cast<Standard_Real>(uu);
  Handle(Geom_Curve) cc = _ptr->UIso(uR);

  Handle(Geom_BezierCurve) ccBezier = Handle(Geom_BezierCurve)::DownCast(cc);
  Handle(Geom_BSplineCurve) ccBSpline = Handle(Geom_BSplineCurve)::DownCast(cc);

  Standard_Real v1R = static_cast<Standard_Real>(vvMin);
  Standard_Real v2R = static_cast<Standard_Real>(vvMax);

  if (!ccBezier.IsNull())
  {
    ccBezier->Segment(v1R, v2R);

    dtOCCCurveBase base;
    base.setOCC(ccBezier);

    return new dtOCCBezierCurve(base);
  }
  else if (!ccBSpline.IsNull())
  {
    ccBSpline->Segment(v1R, v2R);

    dtOCCCurveBase base;
    base.setOCC(ccBSpline);

    return new dtOCCBSplineCurve(base);
  }
  else
  {
    dt__throw(
      segmentConstU(),
      << dt__eval(ccBezier.get()) << std::endl
      << dt__eval(ccBSpline.get())
    );
  }
}

dtCurve *dtOCCSurfaceOfRevolution::segmentConstV(
  dtReal const vv, dtReal const uuMin, dtReal const uuMax
) const
{
  Standard_Real vR = static_cast<Standard_Real>(vv);
  Handle(Geom_Curve) cc = _ptr->VIso(vR);

  Handle(Geom_BezierCurve) ccBezier = Handle(Geom_BezierCurve)::DownCast(cc);
  Handle(Geom_BSplineCurve) ccBSpline = Handle(Geom_BSplineCurve)::DownCast(cc);
  Handle(Geom_Conic) ccConic = Handle(Geom_Conic)::DownCast(cc);

  Standard_Real u1R = static_cast<Standard_Real>(uuMin);
  Standard_Real u2R = static_cast<Standard_Real>(uuMax);

  if (!ccBezier.IsNull())
  {
    ccBezier->Segment(u1R, u2R);

    dtOCCCurveBase base;
    base.setOCC(ccBezier);

    return new dtOCCBezierCurve(base);
  }
  else if (!ccBSpline.IsNull())
  {
    ccBSpline->Segment(u1R, u2R);

    dtOCCCurveBase base;
    base.setOCC(ccBSpline);

    return new dtOCCBSplineCurve(base);
  }
  else if (!ccConic.IsNull())
  {
    if (u1R < ccConic->FirstParameter())
    {
      u1R = ccConic->FirstParameter();
    }
    if (u2R > ccConic->LastParameter())
    {
      u2R = ccConic->LastParameter();
    }
    Handle(Geom_TrimmedCurve) trim = new Geom_TrimmedCurve(ccConic, u1R, u2R);
    dtOCCCurveBase base;
    base.setOCC(trim);

    return new dtOCCTrimmedCurve(base);
  }
  else
  {
    dt__throw(
      segmentConstV(),
      << dt__eval(ccBezier.get()) << std::endl
      << dt__eval(ccBSpline.get()) << std::endl
      << dt__eval(ccConic.get())
    );
  }
}
} // namespace dtOO
