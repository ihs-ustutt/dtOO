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

#include "geomCurve2d_curve2dTranslateConstructOCC.h"
#include "geomCurve2d_baseConstructOCC.h"
#include <geometryEngine/dtOCCCurve2d.h>
#include <geometryEngine/dtOCCCurve2dBase.h>

#include <Geom2d_Curve.hxx>
#include <gp_Vec2d.hxx>

namespace dtOO {
geomCurve2d_curve2dTranslateConstructOCC ::
  geomCurve2d_curve2dTranslateConstructOCC()
{
}

geomCurve2d_curve2dTranslateConstructOCC ::
  geomCurve2d_curve2dTranslateConstructOCC(
    dtCurve2d const *const dtC2d, dtVector2 const &vv
  )
{
  dt__ptrAss(dtOCCCurve2d const *const cc, dtOCCCurve2d::ConstDownCast(dtC2d));

  Geom2d_Curve const *cP;
  dt__mustCast(cc->OCCRef().getOCC().get(), Geom2d_Curve const, cP);

  Handle(Geom2d_Curve) aRes = Handle(Geom2d_Curve)::DownCast(cP->Copy());
  aRes->Translate(gp_Vec2d(
    static_cast<Standard_Real>(vv.x()), static_cast<Standard_Real>(vv.y())
  ));

  //
  // create new curve
  //
  dtOCCCurve2dBase base(aRes);
  _dtC2d.reset(geomCurve2d_baseConstructOCC(base).result());
}

geomCurve2d_curve2dTranslateConstructOCC ::
  ~geomCurve2d_curve2dTranslateConstructOCC()
{
}

dtCurve2d *geomCurve2d_curve2dTranslateConstructOCC::result(void)
{
  return _dtC2d->clone();
}
} // namespace dtOO
