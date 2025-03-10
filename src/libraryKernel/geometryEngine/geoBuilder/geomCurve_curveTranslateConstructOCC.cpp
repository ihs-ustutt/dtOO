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

#include "geomCurve_curveTranslateConstructOCC.h"
#include "geomCurve_baseConstructOCC.h"
#include <geometryEngine/dtOCCCurve.h>
#include <geometryEngine/dtOCCCurveBase.h>

#include <Geom_Curve.hxx>
#include <gp_Vec.hxx>

namespace dtOO {
geomCurve_curveTranslateConstructOCC::geomCurve_curveTranslateConstructOCC() {}

geomCurve_curveTranslateConstructOCC::geomCurve_curveTranslateConstructOCC(
  dtCurve const *const dtC, dtVector3 const &vv
)
{
  dt__ptrAss(dtOCCCurve const *const cc, dtOCCCurve::ConstDownCast(dtC));

  Geom_Curve const *cP;
  dt__mustCast(cc->OCCRef().getOCC().get(), Geom_Curve const, cP);

  Handle(Geom_Curve) aRes = Handle(Geom_Curve)::DownCast(cP->Copy());
  aRes->Translate(gp_Vec(
    static_cast<Standard_Real>(vv.x()),
    static_cast<Standard_Real>(vv.y()),
    static_cast<Standard_Real>(vv.z())
  ));

  //
  // create new curve
  //
  dtOCCCurveBase base(aRes);
  _dtC.reset(geomCurve_baseConstructOCC(base).result());
}

geomCurve_curveTranslateConstructOCC::~geomCurve_curveTranslateConstructOCC() {}

dtCurve *geomCurve_curveTranslateConstructOCC::result(void)
{
  return _dtC->clone();
}
} // namespace dtOO
