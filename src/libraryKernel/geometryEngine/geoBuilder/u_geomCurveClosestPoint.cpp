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

#include "u_geomCurveClosestPoint.h"
#include "logMe/logMe.h"

#include <attributionHeaven/pointCurveDist.h>
#include <geometryEngine/dtCurve.h>
#include <gslMinFloatAttr.h>
#include <interfaceHeaven/staticPropertiesHandler.h>

namespace dtOO {
u_geomCurveClosestPoint::u_geomCurveClosestPoint(
  dtCurve const *const dtC, dtPoint3 const &pXYZ
)
{
  dtReal const xyzResolution =
    staticPropertiesHandler::getInstance()->getOptionFloat("xyz_resolution");

  //
  // minimize
  //
  gslMinFloatAttr gmf(
    dt__pH(pointCurveDist)(new pointCurveDist(pXYZ, dtC)),
    0.5,
    0.001,
    xyzResolution
  );
  gmf.perform();

  _closestU = dtC->u_uPercent(gmf.result()[0]);
}

u_geomCurveClosestPoint::~u_geomCurveClosestPoint() {}

dtReal u_geomCurveClosestPoint::result(void) { return _closestU; }
} // namespace dtOO
