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

#include "bSplineCurve_bSplineCurveSplitConstructOCC.h"

#include <logMe/logMe.h>
#include <progHelper.h>

#include <geometryEngine/dtOCCBSplineCurve.h>
#include <geometryEngine/dtOCCCurveBase.h>

#include <GeomConvert.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Precision.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_Failure.hxx>
#include <Standard_TypeDef.hxx>

namespace dtOO {
bSplineCurve_bSplineCurveSplitConstructOCC ::
  bSplineCurve_bSplineCurveSplitConstructOCC(
    dtCurve const *const dtC, dtReal const &u0, dtReal const &u1
  )
{
  dt__ptrAss(
    dtOCCBSplineCurve const *const bS, dtOCCBSplineCurve::ConstDownCast(dtC)
  );

  Handle(Geom_BSplineCurve) geomBSpline =
    Handle(Geom_BSplineCurve)::DownCast(bS->OCCRef().getOCC());

  dt__throwIf(
    geomBSpline.IsNull(), bSplineCurve_bSplineCurveSplitConstructOCC()
  );

  dtOCCCurveBase base;

  dt__tryOcc(base.setOCC(GeomConvert::SplitBSplineCurve(
    geomBSpline, Standard_Real(u0), Standard_Real(u1), Precision::Confusion()
  ));
             , << "Split fails.");
  _dtC.reset(new dtOCCBSplineCurve(base));
}

bSplineCurve_bSplineCurveSplitConstructOCC ::
  ~bSplineCurve_bSplineCurveSplitConstructOCC()
{
}

dtCurve *bSplineCurve_bSplineCurveSplitConstructOCC::result(void)
{
  return _dtC->clone();
}
} // namespace dtOO
