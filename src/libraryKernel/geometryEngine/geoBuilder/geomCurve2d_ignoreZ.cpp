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

#include "geomCurve2d_ignoreZ.h"
#include "interfaceHeaven/vectorHandling.h"
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtCurve2d.h>
#include <geometryEngine/dtOCCBSplineCurve.h>
#include <geometryEngine/dtOCCBSplineCurve2d.h>
#include <geometryEngine/dtOCCCurve2d.h>
#include <geometryEngine/dtOCCCurve2dBase.h>
#include <geometryEngine/dtOCCCurveBase.h>

#include "bSplineCurve2d_pointConstructOCC.h"
#include <Geom_BSplineCurve.hxx>
#include <Geom_Curve.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_Failure.hxx>
#include <Standard_TypeDef.hxx>

namespace dtOO {
geomCurve2d_ignoreZ::geomCurve2d_ignoreZ(dtCurve const *const dtC)
{
  dt__ptrAss(dtOCCCurve const *const occC, dtOCCCurve::ConstDownCast(dtC));

  std::vector<dtPoint2> p2;
  for (int ii = 0; ii < occC->nControlPoints(); ii++)
  {
    p2.push_back(dtLinearAlgebra::ignoreZ(occC->controlPoint(ii)));
  }
  if (dtOCCBSplineCurve::ConstDownCast(occC))
  {
    _dtC2d.reset(bSplineCurve2d_pointConstructOCC(p2, occC->order()).result());
  }
  else
  {
    dt__throw(geomCurve2d_ignoreZ(), << "Unknown OCC type.");
  }
}

geomCurve2d_ignoreZ::~geomCurve2d_ignoreZ() {}

dtCurve2d *geomCurve2d_ignoreZ::result(void) { return _dtC2d->clone(); }
} // namespace dtOO
