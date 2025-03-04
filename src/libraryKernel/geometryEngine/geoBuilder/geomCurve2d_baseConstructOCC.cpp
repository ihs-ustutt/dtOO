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

#include "geomCurve2d_baseConstructOCC.h"
#include <logMe/dtMacros.h>

#include <geometryEngine/dtCurve2d.h>
#include <geometryEngine/dtOCCBSplineCurve2d.h>
#include <geometryEngine/dtOCCCurve2d.h>
#include <geometryEngine/dtOCCCurve2dBase.h>
#include <geometryEngine/dtOCCLine2d.h>
#include <geometryEngine/dtOCCTrimmedCurve2d.h>
#include <logMe/logMe.h>

#include <Geom2d_BSplineCurve.hxx>
#include <Geom2d_Curve.hxx>
#include <Geom2d_Line.hxx>
#include <Geom2d_TrimmedCurve.hxx>
#include <Precision.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_Failure.hxx>
#include <Standard_TypeDef.hxx>

namespace dtOO {
geomCurve2d_baseConstructOCC::geomCurve2d_baseConstructOCC(
  dtOCCCurve2dBase const &base
)
{
  Handle(Geom2d_Curve) occC = base.getOCC();
  if (!(Handle(Geom2d_BSplineCurve)::DownCast(occC).IsNull()))
  {
    _dtC2d.reset(new dtOCCBSplineCurve2d(base));
  }
  else if (!(Handle(Geom2d_TrimmedCurve)::DownCast(occC).IsNull()))
  {
    _dtC2d.reset(new dtOCCTrimmedCurve2d(
      base, occC->FirstParameter(), occC->LastParameter()
    ));
  }
  else if (!(Handle(Geom2d_Line)::DownCast(occC).IsNull()))
  {
    _dtC2d.reset(new dtOCCLine2d(base));
  }
  else
  {
    dt__throw(
      geomCurve2d_baseConstructOCC(),
      << dt__eval(occC->DynamicType()->Name()) << std::endl
      << "Unsupported OCC type."
    );
  }
}

geomCurve2d_baseConstructOCC::~geomCurve2d_baseConstructOCC() {}

dtCurve2d *geomCurve2d_baseConstructOCC::result(void)
{
  return _dtC2d->clone();
}
} // namespace dtOO
