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

#include "bSplineCurve_convertOCC.h"
#include <logMe/dtMacros.h>

#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtOCCBSplineCurve.h>
#include <geometryEngine/dtOCCBezierCurve.h>
#include <geometryEngine/dtOCCCurve.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCLine.h>
#include <geometryEngine/dtOCCTrimmedCurve.h>
#include <logMe/logMe.h>

#include <GeomConvert.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_Curve.hxx>
#include <Precision.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_Failure.hxx>
#include <Standard_TypeDef.hxx>
// #include <Geom_BezierCurve.hxx>
// #include <Geom_TrimmedCurve.hxx>
// #include <Geom_Line.hxx>

namespace dtOO {
bSplineCurve_convertOCC::bSplineCurve_convertOCC(dtCurve const &dtC)
{
  //
  // check if geometry is an OCC geometry
  //
  dtOCCCurve const *const dtOccC = dtOCCCurve::MustConstDownCast(&dtC);

  //
  // check if it is already a bSpline
  //
  if (dtOCCBSplineCurve::ConstDownCast(dtOccC))
  {
    _dtC.reset(dtC.clone());
  }
  else
  {
    Handle(Geom_Curve) occC = dtOccC->OCCRef().getOCC();

    Handle(Geom_BSplineCurve) bSC;
    dt__tryOcc(bSC = GeomConvert::CurveToBSplineCurve(occC);
               , << "Cannot convert to Geom_BSplineCurve.");
    dtOCCCurveBase cBase;
    cBase.setOCC(bSC);
    _dtC.reset(new dtOCCBSplineCurve(cBase));
  }
}

bSplineCurve_convertOCC::~bSplineCurve_convertOCC() {}

dtCurve *bSplineCurve_convertOCC::result(void) { return _dtC->clone(); }
} // namespace dtOO
