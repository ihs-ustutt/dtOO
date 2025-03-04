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

#include "geomCurve_curveRotateConstructOCC.h"
#include "geomCurve_baseConstructOCC.h"
#include <geometryEngine/dtOCCCurve.h>
#include <geometryEngine/dtOCCCurveBase.h>

#include <Geom_Curve.hxx>
#include <gp_Ax1.hxx>
#include <gp_Dir.hxx>
#include <gp_Pnt.hxx>

namespace dtOO {
geomCurve_curveRotateConstructOCC::geomCurve_curveRotateConstructOCC() {}

geomCurve_curveRotateConstructOCC::geomCurve_curveRotateConstructOCC(
  dtCurve const *const dtC,
  dtPoint3 const &oP,
  dtVector3 const &rA,
  dtReal const &angle
)
{
  dt__ptrAss(dtOCCCurve const *const cc, dtOCCCurve::ConstDownCast(dtC));

  Geom_Curve const *cP;
  dt__mustCast(cc->OCCRef().getOCC().get(), Geom_Curve const, cP);

  gp_Pnt pp(
    static_cast<Standard_Real>(oP.x()),
    static_cast<Standard_Real>(oP.y()),
    static_cast<Standard_Real>(oP.z())
  );
  gp_Dir dir(
    static_cast<Standard_Real>(rA.x()),
    static_cast<Standard_Real>(rA.y()),
    static_cast<Standard_Real>(rA.z())
  );
  gp_Ax1 rotAx(pp, dir);
  Standard_Real angleR = static_cast<Standard_Real>(angle);

  Handle(Geom_Curve) aRes = Handle(Geom_Curve)::DownCast(cP->Copy());
  aRes->Rotate(rotAx, angleR);

  //
  // create new surface
  //
  dtOCCCurveBase base(aRes);
  _dtC.reset(geomCurve_baseConstructOCC(base).result());
}

geomCurve_curveRotateConstructOCC::~geomCurve_curveRotateConstructOCC() {}

dtCurve *geomCurve_curveRotateConstructOCC::result(void)
{
  return _dtC->clone();
}
} // namespace dtOO
