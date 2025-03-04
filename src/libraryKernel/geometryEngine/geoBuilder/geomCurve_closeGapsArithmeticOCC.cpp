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

#include "geomCurve_closeGapsArithmeticOCC.h"
#include <geometryEngine/dtCurve.h>

namespace dtOO {
geomCurve_closeGapsArithmeticOCC::geomCurve_closeGapsArithmeticOCC() {}

geomCurve_closeGapsArithmeticOCC::geomCurve_closeGapsArithmeticOCC(
  dt__pVH(dtCurve) const &dtC
)
{
  //
  // get first and last point of each curve and create a pair
  //
  typedef std::vector<std::pair<dtVector3, dtVector3>> pPair;
  pPair pp;
  dt__forAllConstIter(dt__pVH(dtCurve), dtC, it)
  {
    dtCurve const &theCurve = *it;
    pp.push_back(std::pair<dtVector3, dtVector3>(
      dtLinearAlgebra::toDtVector3(theCurve.controlPoint(0)),
      dtLinearAlgebra::toDtVector3(
        theCurve.controlPoint(theCurve.nControlPoints() - 1)
      )
    ));
  }
  pp.push_back(pp[0]);

  //
  // average inner elements of curve vector
  //
  dt__forInnerIndex(pp, ii)
  {
    pp[ii].first = 0.5 * (pp[ii].first + pp[ii - 1].second);
    pp[ii - 1].second = pp[ii].first;

    pp[ii].second = 0.5 * (pp[ii + 1].first + pp[ii].second);
    pp[ii + 1].first = pp[ii].second;
  }

  //
  // handle first and last element
  //
  pp.front().first = pp.back().first;
  pp.pop_back();

  //
  // move control points of curves
  //
  dt__forAllConstIter(dt__pVH(dtCurve), dtC, it)
  {
    int ii = it - dtC.begin();
    dtCurve const &theCurve = *it;
    dtCurve *theClone = theCurve.clone();
    theClone->setControlPoint(0, dtLinearAlgebra::toDtPoint3(pp[ii].first));
    theClone->setControlPoint(
      theClone->nControlPoints() - 1, dtLinearAlgebra::toDtPoint3(pp[ii].second)
    );
    _dtC.push_back(theClone);
  }
}

geomCurve_closeGapsArithmeticOCC::~geomCurve_closeGapsArithmeticOCC() {}

dt__pVH(dtCurve) geomCurve_closeGapsArithmeticOCC::result(void) { return _dtC; }
} // namespace dtOO
