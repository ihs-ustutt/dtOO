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

#include "map1dTo3d_orderLoop.h"
#include "dtLinearAlgebra.h"
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

#include <analyticGeometryHeaven/map1dTo3d.h>

namespace dtOO {
map1dTo3d_orderLoop::map1dTo3d_orderLoop(dt__pVH(map1dTo3d) const &m1ds)
{
  logContainer<map1dTo3d_orderLoop> logC(logDEBUG, "map1dTo3d_orderLoop()");
  // clone pointer vector
  dt__pVH(map1dTo3d) m1ds_c = m1ds;
  // transfer first object to class pointer vector object
  _m1ds.transfer(_m1ds.end(), m1ds_c.begin(), m1ds_c);
  logC() << "Ordering " << m1ds.size() << " curves." << std::endl;
  while (!m1ds_c.empty())
  {
    // get end point of the last curve in the sorted pointer vector
    dtPoint3 const endPoint = _m1ds.back().getPointPercent(1.0);
    bool success = false;
    dt__forAllIterAuto(m1ds_c, it)
    {
      if (analyticGeometry::inXYZTolerance(endPoint, it->getPointPercent(0.0)))
      {
        _m1ds.transfer(_m1ds.end(), it, m1ds_c);
        success = true;
        logC() << "Add curve. Remaining curves " << m1ds_c.size() << std::endl;
        break;
      }
      else if (analyticGeometry::inXYZTolerance(
                 endPoint, it->getPointPercent(1.0)
               ))
      {
        dt__throwUnexpected(map1dTo3d_orderLoop());
      }
    }
    dt__throwIf(!success, map1dTo3d_orderLoop());
  }
}

map1dTo3d_orderLoop::~map1dTo3d_orderLoop() {}

dt__pVH(map1dTo3d) map1dTo3d_orderLoop::result(void) { return _m1ds; }
} // namespace dtOO
