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

#include "bSplineCurve2d_angleDeltaXDeltaYConstructOCC.h"
#include "progHelper.h"

#include "bSplineCurve2d_pointConstructOCC.h"
#include <geometryEngine/dtCurve2d.h>
#include <logMe/logMe.h>

namespace dtOO {
bSplineCurve2d_angleDeltaXDeltaYConstructOCC ::
  bSplineCurve2d_angleDeltaXDeltaYConstructOCC(
    dtReal angleIn, dtReal angleOut, dtReal const &deltaX, dtReal const &deltaY
  )
{
  //
  // check values
  //
  if (angleIn < 0.)
  {
    dt__warning(
      bSplineCurve2d_angleDeltaXDeltaYConstructOCC(),
      << "Adjusting angleIn from " << dt__eval(angleIn) << " to " << 0. << "."
    );
    angleIn = 0.;
  }
  if (angleOut < 0.)
  {
    dt__warning(
      bSplineCurve2d_angleDeltaXDeltaYConstructOCC(),
      << "Adjusting angleOut from " << dt__eval(angleOut) << " to " << 0. << "."
    );
    angleOut = 0.;
  }
  if (angleIn == angleOut)
  {
    //
    // calculate points
    //
    std::vector<dtPoint2> pV;
    pV.push_back(dtPoint2(0., 0.));
    pV.push_back(dtPoint2(0.5 * deltaX, 0.5 * deltaY));
    pV.push_back(dtPoint2(deltaX, deltaY));

    dt__info(
      bSplineCurve2d_angleDeltaXDeltaYConstructOCC(),
      << dt__eval(angleIn) << std::endl
      << dt__eval(angleOut) << std::endl
      << dt__eval(deltaX) << std::endl
      << dt__eval(deltaY) << std::endl
      << dt__point2d(pV[0]) << std::endl
      << dt__point2d(pV[1]) << std::endl
      << dt__point2d(pV[2])
    );

    _dtC2d.reset(bSplineCurve2d_pointConstructOCC(pV, 1).result());
  }
  else
  {
    //
    // calculate points
    //
    std::vector<dtPoint2> pV;
    pV.push_back(dtPoint2(0., 0.));
    dtReal ss = (deltaY - deltaX * sin(angleIn) / cos(angleIn)) /
                (sin(angleOut) - (cos(angleOut) * sin(angleIn) / cos(angleIn)));
    dtReal tt = (deltaY - deltaX * sin(angleOut) / cos(angleOut)) /
                (sin(angleIn) - (cos(angleIn) * sin(angleOut) / cos(angleOut)));

    pV.push_back(dtPoint2(cos(angleIn) * tt, sin(angleIn) * tt));
    pV.push_back(dtPoint2(deltaX, deltaY));

    dt__info(
      bSplineCurve2d_angleDeltaXDeltaYConstructOCC(),
      << dt__eval(angleIn) << " rad ( " << angleIn * (180. / M_PI) << " deg )"
      << std::endl
      << dt__eval(angleOut) << " rad ( " << angleOut * (180. / M_PI) << " deg )"
      << std::endl
      << dt__eval(ss) << std::endl
      << dt__eval(tt) << std::endl
      << dt__eval(deltaX) << std::endl
      << dt__eval(deltaY) << std::endl
      << dt__point2d(pV[0]) << std::endl
      << dt__point2d(pV[1]) << std::endl
      << dt__point2d(pV[2])
    );

    //
    // calculate deltaX and check solution
    //
    dtReal deltaXMax;
    dtReal deltaXMin;
    if (angleIn >= angleOut)
    {
      deltaXMax = deltaY / tan(angleOut);
      deltaXMin = deltaY / tan(angleIn);
    }
    else if (angleIn < angleOut)
    {
      deltaXMin = deltaY / tan(angleOut);
      deltaXMax = deltaY / tan(angleIn);
    }
    dtReal ratio = (deltaX - deltaXMin) / (deltaXMax - deltaXMin);
    dt__throwIfWithMessage(
      ratio < 0. || ratio > 1.,
      bSplineCurve2d_angleDeltaXDeltaYConstructOCC(),
      << dt__eval(deltaXMin) << std::endl
      << dt__eval(deltaXMax) << std::endl
      << dt__eval(ratio) << std::endl
      << "deltaX should be bettween deltaXMin and deltaXMax."
    );

    ptrHandling<dtCurve2d> dtC2d(bSplineCurve2d_pointConstructOCC(pV, 2).result(
    ));

    _dtC2d.reset(bSplineCurve2d_pointConstructOCC(pV, 2).result());
  }
}

bSplineCurve2d_angleDeltaXDeltaYConstructOCC ::
  ~bSplineCurve2d_angleDeltaXDeltaYConstructOCC()
{
}

dtCurve2d *bSplineCurve2d_angleDeltaXDeltaYConstructOCC::result(void)
{
  return _dtC2d->clone();
}
} // namespace dtOO
