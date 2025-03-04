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

#include "bSplineCurve_normalOffsetGeomCurveOCC.h"

#include "geometryEngine/dtCurve.h"
#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

namespace dtOO {
bSplineCurve_normalOffsetGeomCurveOCC::bSplineCurve_normalOffsetGeomCurveOCC(
  dtCurve const *const dtC, dtReal const &offset, dtVector3 const &nn
)
{
  dt__throwIf(
    dtC->nControlPoints() < 3, bSplineCurve_normalOffsetGeomCurveOCC()
  );

  int numCP = dtC->nControlPoints();
  vectorHandling<dtPoint3> pp;

  //
  // first point
  //
  dtVector3 vv0 = dtC->controlPoint(numCP - 1) - dtC->controlPoint(numCP - 2);
  dtVector3 vv1 = dtC->controlPoint(1) - dtC->controlPoint(0);
  dtVector3 nC0 = dtLinearAlgebra::crossProduct(vv0, nn);
  nC0 = dtLinearAlgebra::normalize(nC0);
  dtVector3 nC1 = dtLinearAlgebra::crossProduct(vv1, nn);
  nC1 = dtLinearAlgebra::normalize(nC1);
  dtVector3 nC = .5 * (nC0 + nC1);
  nC = dtLinearAlgebra::normalize(nC);

  if (dtC->closed())
  {
    pp.push_back(dtC->controlPoint(0) + offset * nC);
  }
  else
  {
    pp.push_back(dtC->controlPoint(0) + offset * nC1);
  }

  //
  // in-between
  //
  for (int ii = 1; ii < numCP - 1; ii++)
  {
    vv0 = dtC->controlPoint(ii) - dtC->controlPoint(ii - 1);
    vv1 = dtC->controlPoint(ii + 1) - dtC->controlPoint(ii);
    nC0 = dtLinearAlgebra::crossProduct(vv0, nn);
    nC0 = dtLinearAlgebra::normalize(nC0);
    nC1 = dtLinearAlgebra::crossProduct(vv1, nn);
    nC1 = dtLinearAlgebra::normalize(nC1);
    nC = .5 * (nC0 + nC1);
    nC = dtLinearAlgebra::normalize(nC);
    pp.push_back(dtC->controlPoint(ii) + offset * nC);
  }

  //
  // last
  //
  vv0 = dtC->controlPoint(numCP - 1) - dtC->controlPoint(numCP - 2);
  vv1 = dtC->controlPoint(1) - dtC->controlPoint(0);
  nC0 = dtLinearAlgebra::crossProduct(vv0, nn);
  nC0 = dtLinearAlgebra::normalize(nC0);
  nC1 = dtLinearAlgebra::crossProduct(vv1, nn);
  nC1 = dtLinearAlgebra::normalize(nC1);
  nC = .5 * (nC0 + nC1);
  nC = dtLinearAlgebra::normalize(nC);
  if (dtC->closed())
  {
    pp.push_back(pp[0]);
  }
  else
  {
    pp.push_back(dtC->controlPoint(numCP - 1) + offset * nC0);
  }

  //
  // clone and replace control points
  //
  _dtC.reset(dtC->clone());
  dt__forFromToIndex(0, numCP, ii) _dtC->setControlPoint(ii, pp[ii]);
}

bSplineCurve_normalOffsetGeomCurveOCC ::~bSplineCurve_normalOffsetGeomCurveOCC()
{
}

dtCurve *bSplineCurve_normalOffsetGeomCurveOCC::result(void)
{
  return _dtC->clone();
}
} // namespace dtOO
