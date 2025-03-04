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

#include "geomCurve_curveReverseConstructOCC.h"
#include "geomCurve_baseConstructOCC.h"
#include <geometryEngine/dtOCCCurve.h>
#include <geometryEngine/dtOCCCurveBase.h>

#include <Geom_Curve.hxx>

namespace dtOO {
geomCurve_curveReverseConstructOCC::geomCurve_curveReverseConstructOCC() {}

geomCurve_curveReverseConstructOCC::geomCurve_curveReverseConstructOCC(
  dtCurve const *const dtC, bool const &revU
)
{
  dt__ptrAss(dtOCCCurve const *const cc, dtOCCCurve::ConstDownCast(dtC));

  Geom_Curve const *cP;
  dt__mustCast(cc->OCCRef().getOCC().get(), Geom_Curve const, cP);

  Handle(Geom_Curve) aRes = Handle(Geom_Curve)::DownCast(cP->Copy());
  if (revU)
    aRes->Reverse();

  //
  // create new curve
  //
  dtOCCCurveBase base(aRes);
  _dtC.reset(geomCurve_baseConstructOCC(base).result());
}

geomCurve_curveReverseConstructOCC::~geomCurve_curveReverseConstructOCC() {}

dtCurve *geomCurve_curveReverseConstructOCC::result(void)
{
  return _dtC->clone();
}
} // namespace dtOO
