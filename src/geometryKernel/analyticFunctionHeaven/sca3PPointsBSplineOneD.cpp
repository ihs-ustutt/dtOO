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

#include "sca3PPointsBSplineOneD.h"

#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

#include "analyticFunctionTransformed.h"
#include <boost/assign/list_of.hpp>
#include <dtLinearAlgebra.h>
#include <geometryEngine/dtCurve2d.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_pointConstructOCC.h>
#include <math.h>

namespace dtOO {
sca3PPointsBSplineOneD::sca3PPointsBSplineOneD()
  : scaOneDPolyInterface(),
    _curve(dt__tmpPtr(
      dtCurve2d,
      bSplineCurve2d_pointConstructOCC(
        // clang-format off
        ::boost::assign::list_of
          (dtPoint2(0.0, 0.0))
          (dtPoint2(0.5, 0.5))
          (dtPoint2(1.0, 1.0)),
        // clang-format on
        2
      )
        .result()
    ))
{
  setMinMax(0., 1.);
}

sca3PPointsBSplineOneD::sca3PPointsBSplineOneD(
  sca3PPointsBSplineOneD const &orig
)
  : scaOneDPolyInterface(orig), _curve(orig._curve)
{
}

sca3PPointsBSplineOneD::sca3PPointsBSplineOneD(
  dtReal const &xB, dtReal const &yB, dtReal const &xA, dtReal const &yA
)
  : scaOneDPolyInterface(),
    _curve(dt__tmpPtr(
      dtCurve2d,
      bSplineCurve2d_pointConstructOCC(
        // clang-format off
        ::boost::assign::list_of
          (dtPoint2(0.0, 0.0))
          (dtPoint2(xB, yB))
          (dtPoint2(0.5 * (xB + xA), 0.5 * (yB + yA)))
          (dtPoint2(xA, yA))
          (dtPoint2(1.0, 1.0)),
        // clang-format on
        2
      )
        .result()
    ))
{
  setMinMax(0., 1.);
}

sca3PPointsBSplineOneD::~sca3PPointsBSplineOneD() {}

sca3PPointsBSplineOneD *sca3PPointsBSplineOneD::clone(void) const
{
  return new sca3PPointsBSplineOneD(*this);
}

sca3PPointsBSplineOneD *
sca3PPointsBSplineOneD::cloneTransformed(dtTransformer const *const dtT) const
{
  return new analyticFunctionTransformed<sca3PPointsBSplineOneD>(*this, dtT);
}

sca3PPointsBSplineOneD *sca3PPointsBSplineOneD::create(void) const
{
  return new sca3PPointsBSplineOneD();
}

dtReal sca3PPointsBSplineOneD::YFloat(dtReal const &xx) const
{
  return _curve.YFloat(xx);
}

dtInt sca3PPointsBSplineOneD::nDOF(void) const { return 2; }

void sca3PPointsBSplineOneD::setDOF(std::vector<dtReal> const value)
{
  dt__throwIf(value.size() != nDOF(), setDOF());
  dt__throwIf(value[0] < 0.0 || value[0] > 1.0, setDOF());
  dt__throwIf(value[1] < 0.0 || value[1] > 1.0, setDOF());

  dtInt const midCP = (_curve.constPtrDtCurve2d()->nControlPoints() - 1) / 2;
  _curve.ptrDtCurve2d()->setControlPoint(
    midCP,
    dtPoint2(
      _curve.constPtrDtCurve2d()->controlPoint(midCP - 1).x() +
        value[0] * (_curve.constPtrDtCurve2d()->controlPoint(midCP + 1).x() -
                    _curve.constPtrDtCurve2d()->controlPoint(midCP - 1).x()),
      _curve.constPtrDtCurve2d()->controlPoint(midCP - 1).y() +
        value[1] * (_curve.constPtrDtCurve2d()->controlPoint(midCP + 1).y() -
                    _curve.constPtrDtCurve2d()->controlPoint(midCP - 1).y())
    )
  );
}
} // namespace dtOO
