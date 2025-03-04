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

#include "geomCurve_baseConstructOCC.h"
#include <logMe/dtMacros.h>

#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtOCCBSplineCurve.h>
#include <geometryEngine/dtOCCBezierCurve.h>
#include <geometryEngine/dtOCCCircle.h>
#include <geometryEngine/dtOCCCurve.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCEllipse.h>
#include <geometryEngine/dtOCCLine.h>
#include <geometryEngine/dtOCCTrimmedCurve.h>
#include <logMe/logMe.h>

#include <Geom_BSplineCurve.hxx>
#include <Geom_BezierCurve.hxx>
#include <Geom_Circle.hxx>
#include <Geom_Curve.hxx>
#include <Geom_Ellipse.hxx>
#include <Geom_Line.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <Precision.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_Failure.hxx>
#include <Standard_TypeDef.hxx>

namespace dtOO {
geomCurve_baseConstructOCC::geomCurve_baseConstructOCC(
  dtOCCCurveBase const &base
)
{
  Handle(Geom_Curve) occC = base.getOCC();
  if (!(Handle(Geom_BSplineCurve)::DownCast(occC).IsNull()))
  {
    _dtC.reset(new dtOCCBSplineCurve(base));
  }
  else if (!(Handle(Geom_BezierCurve)::DownCast(occC).IsNull()))
  {
    _dtC.reset(new dtOCCBezierCurve(base));
  }
  else if (!(Handle(Geom_Circle)::DownCast(occC).IsNull()))
  {
    _dtC.reset(new dtOCCCircle(base));
  }
  else if (!(Handle(Geom_Ellipse)::DownCast(occC).IsNull()))
  {
    _dtC.reset(new dtOCCEllipse(base));
  }
  else if (!(Handle(Geom_TrimmedCurve)::DownCast(occC).IsNull()))
  {
    _dtC.reset(new dtOCCTrimmedCurve(base));
  }
  else if (!(Handle(Geom_Line)::DownCast(occC).IsNull()))
  {
    _dtC.reset(new dtOCCLine(base));
  }
  else
  {
    dt__throw(
      geomCurve_baseConstructOCC(),
      << dt__eval(occC->DynamicType()->Name()) << std::endl
      << "Unsupported OCC type."
    );
  }
}

geomCurve_baseConstructOCC::~geomCurve_baseConstructOCC() {}

dtCurve *geomCurve_baseConstructOCC::result(void) { return _dtC->clone(); }
} // namespace dtOO
