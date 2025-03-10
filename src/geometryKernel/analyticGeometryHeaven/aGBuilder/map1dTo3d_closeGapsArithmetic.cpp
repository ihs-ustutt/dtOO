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

#include "map1dTo3d_closeGapsArithmetic.h"

#include <analyticFunctionHeaven/vec2dCurve2dOneD.h>
#include <analyticFunctionHeaven/vec2dOneD.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/vec2dOneDInMap2dTo3d.h>
#include <geometryEngine/dtCurve2d.h>
#include <geometryEngine/geoBuilder/geomCurve2d_closeGapsArithmeticOCC.h>

namespace dtOO {
map1dTo3d_closeGapsArithmetic::map1dTo3d_closeGapsArithmetic(dt__pVH(map1dTo3d)
                                                               const &m1d)
{
  //
  // casting and fill arraies
  //
  dt__pVH(dtCurve2d) dtC2d;
  dt__forAllConstIter(dt__pVH(map1dTo3d), m1d, it)
  {
    map1dTo3d const &theMap = *it;
    //
    // casting
    //
    dt__ptrAss(
      vec2dOneDInMap2dTo3d const *const v2dInM3d,
      vec2dOneDInMap2dTo3d::ConstDownCast(&theMap)
    );
    dt__ptrAss(
      vec2dCurve2dOneD const *const v2dC1d,
      vec2dCurve2dOneD::ConstDownCast(v2dInM3d->ptrToVec2dOneD())
    );
    //			map2dTo3d const * const m2d =
    // v2dInM3d->ptrToMap2dTo3d();
    dtC2d.push_back(v2dC1d->ptrDtCurve2d()->clone());
  }

  dtC2d = geomCurve2d_closeGapsArithmeticOCC(dtC2d).result();
  dt__ptrAss(
    vec2dOneDInMap2dTo3d const *const v2dInM3d,
    vec2dOneDInMap2dTo3d::ConstDownCast(&(m1d.front()))
  );

  dt__forAllConstIter(dt__pVH(dtCurve2d), dtC2d, it)
  {
    int ii = it - dtC2d.begin();
    dtCurve2d const &theCurve = *it;
    _m1d.push_back(new vec2dOneDInMap2dTo3d(
      dt__tmpPtr(vec2dCurve2dOneD, new vec2dCurve2dOneD(&theCurve)),
      v2dInM3d->ptrToMap2dTo3d()
    ));
    _m1d.back().setLabel(m1d[ii].getLabel());
  }
}

map1dTo3d_closeGapsArithmetic::~map1dTo3d_closeGapsArithmetic() {}

dt__pVH(map1dTo3d) map1dTo3d_closeGapsArithmetic::result(void) { return _m1d; }
} // namespace dtOO
