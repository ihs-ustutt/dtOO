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

#include "dtOCCRectangularTrimmedSurface.h"

#include "dtOCCBezierCurve.h"
#include "dtOCCCurveBase.h"
#include "dtOCCSurfaceBase.h"
#include "dtOCCTrimmedCurve.h"
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

#include <Geom_RectangularTrimmedSurface.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <Standard_Failure.hxx>
#include <Standard_TypeDef.hxx>

namespace dtOO {
dtOCCRectangularTrimmedSurface::dtOCCRectangularTrimmedSurface()
  : dtOCCSurface()
{
  _ptr = NULL;
}

dtOCCRectangularTrimmedSurface::dtOCCRectangularTrimmedSurface(
  const dtOCCSurfaceBase &orig
)
  : dtOCCSurface(orig)
{
  dt__mustCast(
    OCCRef().getOCC().get(), Geom_RectangularTrimmedSurface const, _ptr
  );
}

dtOCCRectangularTrimmedSurface::~dtOCCRectangularTrimmedSurface() {}

dtOCCRectangularTrimmedSurface *dtOCCRectangularTrimmedSurface::clone(void
) const
{
  return new dtOCCRectangularTrimmedSurface(OCCRef());
}

dtCurve *dtOCCRectangularTrimmedSurface::segmentConstU(
  dtReal const uu, dtReal const vvMin, dtReal const vvMax
) const
{
  Standard_Real uR = static_cast<Standard_Real>(uu);
  Handle(Geom_Curve) cc = _ptr->UIso(uR);

  Handle(Geom_TrimmedCurve) ccB = Handle(Geom_TrimmedCurve)::DownCast(cc);

  Standard_Real v1R = static_cast<Standard_Real>(vvMin);
  Standard_Real v2R = static_cast<Standard_Real>(vvMax);

  ccB->SetTrim(v1R, v2R);

  dtOCCCurveBase base;
  base.setOCC(ccB);

  return new dtOCCTrimmedCurve(base);
}

dtCurve *dtOCCRectangularTrimmedSurface::segmentConstV(
  dtReal const vv, dtReal const uuMin, dtReal const uuMax
) const
{
  Standard_Real vR = static_cast<Standard_Real>(vv);
  Handle(Geom_Curve) cc = _ptr->VIso(vR);

  Handle(Geom_TrimmedCurve) ccB = Handle(Geom_TrimmedCurve)::DownCast(cc);

  Standard_Real u1R = static_cast<Standard_Real>(uuMin);
  Standard_Real u2R = static_cast<Standard_Real>(uuMax);

  ccB->SetTrim(u1R, u2R);

  dtOCCCurveBase base;
  base.setOCC(ccB);

  return new dtOCCTrimmedCurve(base);
}
} // namespace dtOO
