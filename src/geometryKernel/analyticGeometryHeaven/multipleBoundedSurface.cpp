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

#include "multipleBoundedSurface.h"
#include "map1dTo3d.h"
#include "map2dTo3d.h"
#include <analyticGeometryHeaven/aGBuilder/map1dTo3d_orderLoop.h>
//#include <dtTransformerHeaven/dtTransformer.h>
#include <logMe/dtMacros.h>
#include <logMe/logContainer.h>
#include <logMe/logMe.h>

namespace dtOO {
multipleBoundedSurface::multipleBoundedSurface() : analyticGeometry() {}

multipleBoundedSurface::multipleBoundedSurface(
  const multipleBoundedSurface &orig
)
  : analyticGeometry(orig)
{
  _m2d.reset(orig._m2d->clone());
  dt__forAllIndex(orig._m1d, ii) _m1d.push_back(orig._m1d[ii].clone());
  dt__forAllRefAuto(orig._polygon_0, pp) _polygon_0.push_back(pp);
  dt__forAllRefAuto(orig._polygon_1, pp) _polygon_1.push_back(pp);
}

multipleBoundedSurface::~multipleBoundedSurface() {}

multipleBoundedSurface::multipleBoundedSurface(
  analyticGeometry const *const m2d,
  ptrVectorHandling<analyticGeometry> const &m1d
)
{
  _m2d.reset(map2dTo3d::DownCast(m2d->clone()));
  _m1d = map1dTo3d_orderLoop(map1dTo3d::PointerVectorCloneCast(m1d)).result();
  dt__forAllIndex(_m1d, ii)
  {
    _polygon_0.push_back(_m2d->reparamOnFace(_m1d[ii].getPointPercent(0.0)));
    _polygon_1.push_back(_m2d->reparamOnFace(_m1d[ii].getPointPercent(1.0)));
  }
}

multipleBoundedSurface *multipleBoundedSurface::create(void) const
{
  return new multipleBoundedSurface();
}

multipleBoundedSurface *multipleBoundedSurface::clone(void) const
{
  return new multipleBoundedSurface(*this);
}

dtInt multipleBoundedSurface::dim(void) const { return 2; }

bool multipleBoundedSurface::isClosed(dtInt const &dir) const
{
  switch (dir)
  {
  case 0:
    return false;
  case 1:
    return false;
  default:
    dt__throwUnexpected(isClosed());
  }
}

dtReal multipleBoundedSurface::getMin(dtInt const &dir) const
{
  switch (dir)
  {
  case 0:
    return _m2d->getMin(0);
  case 1:
    return _m2d->getMin(1);
  default:
    dt__throwUnexpected(isClosed());
  }
}

dtReal multipleBoundedSurface::getMax(dtInt const &dir) const
{
  switch (dir)
  {
  case 0:
    return _m2d->getMax(0);
  case 1:
    return _m2d->getMax(1);
  default:
    dt__throwUnexpected(isClosed());
  }
}

dtPoint3 multipleBoundedSurface::getPoint(dtReal const *const uvw) const
{
  return _m2d->getPoint(uvw);
}

::std::vector<dtVector3>
multipleBoundedSurface::firstDer(dtReal const *const uvw) const
{
  return _m2d->firstDer(uvw);
}
vectorHandling<renderInterface *> multipleBoundedSurface::getRender(void) const
{
  vectorHandling<renderInterface *> rV;
  dt__forAllIndex(_m1d, ii)
  {
    vectorHandling<renderInterface *> tmpRV = _m1d[ii].getRender();
    rV.insert(rV.end(), tmpRV.begin(), tmpRV.end());
  }

  return rV;
}

ptrVectorHandling<analyticGeometry> const
multipleBoundedSurface::boundsPointerVectorConst(void) const
{
  return analyticGeometry::PointerVectorCloneCast(_m1d);
}

analyticGeometry const *const multipleBoundedSurface::surfaceConstPtr(void
) const
{
  return _m2d.get();
}

bool multipleBoundedSurface::insideInternalPolygon(dtPoint2 const &ppUV) const
{
  return dtLinearAlgebra::isInsidePolygon(_polygon_0[0], _polygon_0);
}
} // namespace dtOO
