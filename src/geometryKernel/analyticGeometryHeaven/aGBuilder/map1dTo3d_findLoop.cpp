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

#include "map1dTo3d_findLoop.h"
#include "dtLinearAlgebra.h"
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

#include <analyticGeometryHeaven/map1dTo3d.h>

namespace dtOO {
map1dTo3d_findLoop::map1dTo3d_findLoop(dt__pVH(map1dTo3d) const &m1ds)
{
  logContainer<map1dTo3d_findLoop> logC(logDEBUG, "map1dTo3d_findLoop()");
  // create index array
  std::vector<dtInt> orientedCurves(m1ds.size());
  dt__forAllIndex(orientedCurves, ii) { orientedCurves[ii] = ii; }
  // add first object to index array
  _orientedCurves.push_back(*(orientedCurves.begin()));
  orientedCurves.erase(orientedCurves.begin());
  logC() << "Ordering " << m1ds.size() << " curves." << std::endl;
  while (!orientedCurves.empty())
  {
    // get end point of the last curve in the sorted pointer vector
    dtPoint3 endPoint;
    if (_orientedCurves.back() >= 0)
    {
      endPoint = m1ds[_orientedCurves.back()].getPointPercent(1.0);
    }
    else
    {
      endPoint = m1ds[abs(_orientedCurves.back())].getPointPercent(0.0);
    }
    bool success = false;
    dt__forAllIterAuto(orientedCurves, it)
    {
      dtInt const pos = it - orientedCurves.begin();
      if (analyticGeometry::inXYZTolerance(
            endPoint, m1ds[*it].getPointPercent(0.0)
          ))
      {
        _orientedCurves.push_back(*it);
        orientedCurves.erase(orientedCurves.begin() + pos);
        success = true;
        break;
      }
      else if (analyticGeometry::inXYZTolerance(
                 endPoint, m1ds[*it].getPointPercent(1.0)
               ))
      {
        _orientedCurves.push_back(-(*it));
        orientedCurves.erase(orientedCurves.begin() + pos);
        success = true;
        break;
      }
    }
    dt__throwIf(!success, map1dTo3d_findLoop());
  }
  logC() << "_orientedCurves = " << _orientedCurves << std::endl;
}

map1dTo3d_findLoop::map1dTo3d_findLoop(
  vectorHandling<analyticGeometry *> const m1ds
)
{
  dt__pVH(map1dTo3d) m1ds_c;
  dt__forAllRefAuto(m1ds, m1d)
  {
    m1ds_c.push_back(map1dTo3d::MustDownCast(m1d->clone()));
  }
  _orientedCurves = map1dTo3d_findLoop(m1ds_c).result();
}

map1dTo3d_findLoop::~map1dTo3d_findLoop() {}

std::vector<dtInt> map1dTo3d_findLoop::result(void) { return _orientedCurves; }
} // namespace dtOO
