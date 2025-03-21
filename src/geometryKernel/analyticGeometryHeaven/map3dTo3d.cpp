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

#include "map3dTo3d.h"

#include "map2dTo3d.h"
#include "vec3dOneDInMap3dTo3d.h"
#include "vec3dTwoDInMap3dTo3d.h"
#include <analyticFunctionHeaven/vec3dBiLinearTwoD.h>
#include <analyticFunctionHeaven/vec3dCurveOneD.h>
#include <analyticFunctionHeaven/vec3dOneD.h>
#include <analyticFunctionHeaven/vec3dSurfaceTwoD.h>
#include <analyticFunctionHeaven/vec3dTwoD.h>
#include <discrete3dVector.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/geoBuilder/bezierSurface_pointConstructOCC.h>
#include <geometryEngine/geoBuilder/trimmedCurve_twoPointsConnectConstructOCC.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <interfaceHeaven/twoDArrayHandling.h>
#include <logMe/dtMacros.h>
#include <logMe/logContainer.h>
#include <logMe/logMe.h>
#include <solid3dLine.h>

#include <attributionHeaven/pointGeometryDist.h>
#include <boost/assign/list_of.hpp>
#include <gslMinFloatAttr.h>

namespace dtOO {
dtReal map3dTo3d::_deltaPer =
  staticPropertiesHandler::getInstance()->getOptionFloat("map3dTo3d_deltaPer");

map3dTo3d::map3dTo3d() : analyticGeometry() {}

map3dTo3d::map3dTo3d(const map3dTo3d &orig) : analyticGeometry(orig) {}

map3dTo3d::~map3dTo3d() {}

dtInt map3dTo3d::dim(void) const { return 3; }

dtPoint3 map3dTo3d::getPoint(dtReal const *const uvw) const
{
  return getPoint(uvw[0], uvw[1], uvw[2]);
}

::std::vector<dtVector3> map3dTo3d::firstDer(dtReal const *const uvw) const
{
  return this->firstDer(uvw[0], uvw[1], uvw[2]);
}

vectorHandling<renderInterface *> map3dTo3d::getRender(void) const
{
  vectorHandling<renderInterface *> rV;
  //
  // calculate interval to create points
  //
  dtReal intU = (getUMax() - getUMin()) / (getRenderResolutionU() - 1);
  dtReal intV = (getVMax() - getVMin()) / (getRenderResolutionV() - 1);
  dtReal intW = (getWMax() - getWMin()) / (getRenderResolutionW() - 1);

  vectorHandling<dtPoint3> pp;
  //
  // get grid
  //
  // v=0, w=0
  for (int ii = 0; ii < getRenderResolutionU(); ii++)
  {
    dtReal uu = (getUMin() + ii * intU);
    dtReal vv = getVMin(); // + jj * intV);
    dtReal ww = getWMin(); // + kk * intW);
    pp.push_back(getPoint(uu, vv, ww));
  }
  rV.push_back(new solid3dLine(pp));
  rV.push_back(new discrete3dVector(
    getPointPercent(0.26, 0.10, 0.10) - getPointPercent(0.25, 0.10, 0.10),
    getPointPercent(0.25, 0.10, 0.10)
  ));
  pp.clear();
  // u=0, w=0
  for (int ii = 0; ii < getRenderResolutionV(); ii++)
  {
    dtReal uu = getUMin(); // + ii * intU);
    dtReal vv = getVMin() + ii * intV;
    dtReal ww = getWMin(); // + kk * intW);
    pp.push_back(getPoint(uu, vv, ww));
  }
  rV.push_back(new solid3dLine(pp));
  rV.push_back(new discrete3dVector(
    getPointPercent(0.10, 0.26, 0.10) - getPointPercent(0.10, 0.25, 0.10),
    getPointPercent(0.10, 0.25, 0.10)
  ));
  rV.push_back(new discrete3dVector(
    getPointPercent(0.10, 0.51, 0.10) - getPointPercent(0.10, 0.50, 0.10),
    getPointPercent(0.10, 0.50, 0.10)
  ));
  pp.clear();
  // u=0, v=0
  for (int ii = 0; ii < getRenderResolutionW(); ii++)
  {
    dtReal uu = getUMin(); // + ii * intU);
    dtReal vv = getVMin(); // + ii * intV;
    dtReal ww = getWMin() + ii * intW;
    pp.push_back(getPoint(uu, vv, ww));
  }
  rV.push_back(new solid3dLine(pp));
  rV.push_back(new discrete3dVector(
    getPointPercent(0.10, 0.10, 0.26) - getPointPercent(0.10, 0.10, 0.25),
    getPointPercent(0.10, 0.10, 0.25)
  ));
  rV.push_back(new discrete3dVector(
    getPointPercent(0.10, 0.10, 0.51) - getPointPercent(0.10, 0.10, 0.50),
    getPointPercent(0.10, 0.10, 0.50)
  ));
  rV.push_back(new discrete3dVector(
    getPointPercent(0.00, 0.00, 0.76) - getPointPercent(0.00, 0.00, 0.75),
    getPointPercent(0.10, 0.10, 0.75)
  ));
  pp.clear();

  // u
  pp.push_back(getPointPercent(0.10, 0.10, 0.10));
  pp.push_back(getPointPercent(0.25, 0.10, 0.10));
  rV.push_back(new solid3dLine(pp));
  pp.clear();
  // v
  pp.push_back(getPointPercent(0.10, 0.10, 0.10));
  pp.push_back(getPointPercent(0.10, 0.25, 0.10));
  rV.push_back(new solid3dLine(pp));
  pp.clear();
  pp.push_back(getPointPercent(0.10, 0.30, 0.10));
  pp.push_back(getPointPercent(0.10, 0.45, 0.10));
  rV.push_back(new solid3dLine(pp));
  pp.clear();
  // v
  pp.push_back(getPointPercent(0.10, 0.10, 0.10));
  pp.push_back(getPointPercent(0.10, 0.10, 0.25));
  rV.push_back(new solid3dLine(pp));
  pp.clear();
  pp.push_back(getPointPercent(0.10, 0.10, 0.30));
  pp.push_back(getPointPercent(0.10, 0.10, 0.45));
  rV.push_back(new solid3dLine(pp));
  pp.clear();
  pp.push_back(getPointPercent(0.10, 0.10, 0.50));
  pp.push_back(getPointPercent(0.10, 0.10, 0.65));
  rV.push_back(new solid3dLine(pp));
  pp.clear();
  // v=0, w=0
  for (int ii = 0; ii < getRenderResolutionU(); ii++)
  {
    dtReal uu = (getUMin() + ii * intU);
    dtReal vv = getVMax(); // + jj * intV);
    dtReal ww = getWMax(); // + kk * intW);
    pp.push_back(getPoint(uu, vv, ww));
  }
  rV.push_back(new solid3dLine(pp));
  pp.clear();
  // u=0, w=0
  for (int ii = 0; ii < getRenderResolutionV(); ii++)
  {
    dtReal uu = getUMax(); // + ii * intU);
    dtReal vv = getVMin() + ii * intV;
    dtReal ww = getWMax(); // + kk * intW);
    pp.push_back(getPoint(uu, vv, ww));
  }
  rV.push_back(new solid3dLine(pp));
  pp.clear();
  // u=0, v=0
  for (int ii = 0; ii < getRenderResolutionW(); ii++)
  {
    dtReal uu = getUMax(); // + ii * intU);
    dtReal vv = getVMax(); // + ii * intV;
    dtReal ww = getWMin() + ii * intW;
    pp.push_back(getPoint(uu, vv, ww));
  }
  rV.push_back(new solid3dLine(pp));
  pp.clear();

  // v=0, w=0
  for (int ii = 0; ii < getRenderResolutionU(); ii++)
  {
    dtReal uu = (getUMin() + ii * intU);
    dtReal vv = getVMin(); // + jj * intV);
    dtReal ww = getWMax(); // + kk * intW);
    pp.push_back(getPoint(uu, vv, ww));
  }
  rV.push_back(new solid3dLine(pp));
  pp.clear();
  // u=0, w=0
  for (int ii = 0; ii < getRenderResolutionV(); ii++)
  {
    dtReal uu = getUMin(); // + ii * intU);
    dtReal vv = getVMin() + ii * intV;
    dtReal ww = getWMax(); // + kk * intW);
    pp.push_back(getPoint(uu, vv, ww));
  }
  rV.push_back(new solid3dLine(pp));
  pp.clear();
  // u=0, v=0
  for (int ii = 0; ii < getRenderResolutionW(); ii++)
  {
    dtReal uu = getUMin(); // + ii * intU);
    dtReal vv = getVMax(); // + ii * intV;
    dtReal ww = getWMin() + ii * intW;
    pp.push_back(getPoint(uu, vv, ww));
  }
  rV.push_back(new solid3dLine(pp));
  pp.clear();

  // v=0, w=0
  for (int ii = 0; ii < getRenderResolutionU(); ii++)
  {
    dtReal uu = (getUMin() + ii * intU);
    dtReal vv = getVMax(); // + jj * intV);
    dtReal ww = getWMin(); // + kk * intW);
    pp.push_back(getPoint(uu, vv, ww));
  }
  rV.push_back(new solid3dLine(pp));
  pp.clear();
  // u=0, w=0
  for (int ii = 0; ii < getRenderResolutionV(); ii++)
  {
    dtReal uu = getUMax(); // + ii * intU);
    dtReal vv = getVMin() + ii * intV;
    dtReal ww = getWMin(); // + kk * intW);
    pp.push_back(getPoint(uu, vv, ww));
  }
  rV.push_back(new solid3dLine(pp));
  pp.clear();
  // u=0, v=0
  for (int ii = 0; ii < getRenderResolutionW(); ii++)
  {
    dtReal uu = getUMax(); // + ii * intU);
    dtReal vv = getVMin(); // + ii * intV;
    dtReal ww = getWMin() + ii * intW;
    pp.push_back(getPoint(uu, vv, ww));
  }
  rV.push_back(new solid3dLine(pp));
  pp.clear();

  return rV;
}

std::vector<dtVector3>
map3dTo3d::firstDer(dtReal const &uu, dtReal const &vv, dtReal const &ww) const
{
  dtReal uP = percent_u(uu);
  dtReal vP = percent_v(vv);
  dtReal wP = percent_w(ww);

  std::vector<dtVector3> ret(3);
  if (uP < _deltaPer)
  {
    ret[0] =
      (getPointPercent(_deltaPer, vP, wP) - getPointPercent(0., vP, wP)) /
      (u_percent(_deltaPer) - u_percent(0.));
  }
  else if ((uP >= _deltaPer) && (uP <= (1. - _deltaPer)))
  {
    ret[0] = (getPointPercent(uP + _deltaPer, vP, wP) -
              getPointPercent(uP - _deltaPer, vP, wP)) /
             (u_percent(uP + _deltaPer) - u_percent(uP - _deltaPer));
  }
  else if (uP > (1. - _deltaPer))
  {
    ret[0] =
      (getPointPercent(1., vP, wP) - getPointPercent(1. - _deltaPer, vP, wP)) /
      (u_percent(1.) - u_percent(1. - _deltaPer));
  }

  if (vP < _deltaPer)
  {
    ret[1] =
      (getPointPercent(uP, _deltaPer, wP) - getPointPercent(uP, 0., wP)) /
      (v_percent(_deltaPer) - v_percent(0.));
  }
  else if ((vP >= _deltaPer) && (vP <= (1. - _deltaPer)))
  {
    ret[1] = (getPointPercent(uP, vP + _deltaPer, wP) -
              getPointPercent(uP, vP - _deltaPer, wP)) /
             (v_percent(vP + _deltaPer) - v_percent(vP - _deltaPer));
  }
  else if (vP > (1. - _deltaPer))
  {
    ret[1] =
      (getPointPercent(uP, 1., wP) - getPointPercent(uP, 1. - _deltaPer, wP)) /
      (v_percent(1.) - v_percent(1. - _deltaPer));
  }

  if (wP < _deltaPer)
  {
    ret[2] =
      (getPointPercent(uP, vP, _deltaPer) - getPointPercent(uP, vP, 0.)) /
      (w_percent(_deltaPer) - w_percent(0.));
  }
  else if ((wP >= _deltaPer) && (wP <= (1. - _deltaPer)))
  {
    ret[2] = (getPointPercent(uP, vP, wP + _deltaPer) -
              getPointPercent(uP, vP, wP - _deltaPer)) /
             (w_percent(wP + _deltaPer) - w_percent(wP - _deltaPer));
  }
  else if (wP > (1. - _deltaPer))
  {
    ret[2] =
      (getPointPercent(uP, vP, 1.) - getPointPercent(uP, vP, 1. - _deltaPer)) /
      (w_percent(1.) - w_percent(1. - _deltaPer));
  }

  return ret;
}

dtVector3
map3dTo3d::firstDerU(dtReal const &uu, dtReal const &vv, dtReal const &ww) const
{
  return firstDer(uu, vv, ww)[0];
}

dtVector3
map3dTo3d::firstDerV(dtReal const &uu, dtReal const &vv, dtReal const &ww) const
{
  return firstDer(uu, vv, ww)[1];
}

dtVector3
map3dTo3d::firstDerW(dtReal const &uu, dtReal const &vv, dtReal const &ww) const
{
  return firstDer(uu, vv, ww)[2];
}

dtPoint3 map3dTo3d::reparamInVolume(dtPoint3 const &ppXYZ) const
{
  return reparamInVolume(ppXYZ, dtVector3(0, 0, 0));
}

dtPoint3 map3dTo3d::reparamPercentInVolume(dtPoint3 const &ppXYZ) const
{
  dtPoint3 ppUVW = reparamInVolume(ppXYZ);

  return percent_uvw(ppUVW);
}

dtPoint3 map3dTo3d::approxPercentInVolume(dtPoint3 const &ppXYZ) const
{
  dtPoint3 ppUVW = approxPercentInVolume(ppXYZ);

  return percent_uvw(ppUVW);
}

dtInt map3dTo3d::getRenderResolutionU(void) const
{
  return analyticGeometry::getRenderResolution(0);
}

dtInt map3dTo3d::getRenderResolutionV(void) const
{
  return analyticGeometry::getRenderResolution(1);
}

dtInt map3dTo3d::getRenderResolutionW(void) const
{
  return analyticGeometry::getRenderResolution(2);
}

bool map3dTo3d::isClosedU(void) const { return isClosed(0); }

bool map3dTo3d::isClosedV(void) const { return isClosed(1); }

bool map3dTo3d::isClosedW(void) const { return isClosed(2); }

dtReal map3dTo3d::getUMin(void) const { return getMin(0); }

dtReal map3dTo3d::getUMax(void) const { return getMax(0); }

dtReal map3dTo3d::getVMin(void) const { return getMin(1); }

dtReal map3dTo3d::getVMax(void) const { return getMax(1); }

dtReal map3dTo3d::getWMin(void) const { return getMin(2); }

dtReal map3dTo3d::getWMax(void) const { return getMax(2); }

bool map3dTo3d::inRange(dtPoint3 const &pUVW) const
{
  if (pUVW.x() > getUMax())
    return false;
  if (pUVW.x() < getUMin())
    return false;
  if (pUVW.y() > getVMax())
    return false;
  if (pUVW.y() < getVMin())
    return false;
  if (pUVW.z() > getWMax())
    return false;
  if (pUVW.z() < getWMin())
    return false;
  return true;
}

dtPoint3 map3dTo3d::getPointPercent(
  dtReal const &uu, dtReal const &vv, dtReal const &ww
) const
{
  return getPoint(u_percent(uu), v_percent(vv), w_percent(ww));
}

dtPoint3 map3dTo3d::getPoint(dtPoint3 const &ppUVW) const
{
  return getPoint(ppUVW.x(), ppUVW.y(), ppUVW.z());
}

dtPoint3 map3dTo3d::getPointPercent(dtPoint3 const &ppUVW) const
{
  return getPoint(uvw_percent(ppUVW));
}

dtReal map3dTo3d::u_percent(dtReal const &uu) const
{
  return (getUMin() + (getUMax() - getUMin()) * uu);
}

dtReal map3dTo3d::v_percent(dtReal const &vv) const
{
  return (getVMin() + (getVMax() - getVMin()) * vv);
}

dtReal map3dTo3d::w_percent(dtReal const &ww) const
{
  return (getWMin() + (getWMax() - getWMin()) * ww);
}

dtReal map3dTo3d::percent_u(dtReal const &uu) const
{
  return ((uu - getUMin()) / (getUMax() - getUMin()));
}

dtReal map3dTo3d::percent_v(dtReal const &vv) const
{
  return ((vv - getVMin()) / (getVMax() - getVMin()));
}

dtReal map3dTo3d::percent_w(dtReal const &ww) const
{
  return ((ww - getWMin()) / (getWMax() - getWMin()));
}

dtPoint3 map3dTo3d::percent_uvw(dtPoint3 const &pUVW) const
{
  return dtPoint3(
    percent_u(pUVW.x()), percent_v(pUVW.y()), percent_w(pUVW.z())
  );
}

dtPoint3 map3dTo3d::uvw_percent(dtPoint3 const &pp) const
{
  return dtPoint3(u_percent(pp.x()), v_percent(pp.y()), w_percent(pp.z()));
}

map1dTo3d *map3dTo3d::segment(dtPoint3 const &p0, dtPoint3 const &p1) const
{
  ptrHandling<dtCurve> dtC(
    trimmedCurve_twoPointsConnectConstructOCC(p0, p1).result()
  );
  vec3dCurveOneD v3d1d(dtC.get());
  return new vec3dOneDInMap3dTo3d(&v3d1d, this);
}

map2dTo3d *map3dTo3d::segment(twoDArrayHandling<dtPoint3> const &pp) const
{
  ptrHandling<dtSurface> dtS(bezierSurface_pointConstructOCC(pp).result());

  vec3dSurfaceTwoD v3d2d(dtS.get());

  return new vec3dTwoDInMap3dTo3d(&v3d2d, this);
}

map2dTo3d *map3dTo3d::segmentConstU(dtReal const &uu) const
{
  dtPoint3 p30(uu, getVMin(), getWMin());
  dtPoint3 p31(uu, getVMax(), getWMin());
  dtPoint3 p32(uu, getVMax(), getWMax());
  dtPoint3 p33(uu, getVMin(), getWMax());

  return segment(p30, p31, p32, p33);
}

map2dTo3d *map3dTo3d::segmentConstV(dtReal const &vv) const
{
  dtPoint3 p30(getUMin(), vv, getWMin());
  dtPoint3 p31(getUMax(), vv, getWMin());
  dtPoint3 p32(getUMax(), vv, getWMax());
  dtPoint3 p33(getUMin(), vv, getWMax());

  return segment(p30, p31, p32, p33);
}

map2dTo3d *map3dTo3d::segmentConstW(dtReal const &ww) const
{
  dtPoint3 p30(getUMin(), getVMin(), ww);
  dtPoint3 p31(getUMax(), getVMin(), ww);
  dtPoint3 p32(getUMax(), getVMax(), ww);
  dtPoint3 p33(getUMin(), getVMax(), ww);

  return segment(p30, p31, p32, p33);
}

/**
 *   (w)
 *    A
 *    |
 *            (1)
 *    +--------+
 *    |        |
 *    |        |
 *    |        |
 *    |        |
 *    +--------+ --> (v)
 *   (0)
 */
map2dTo3d *map3dTo3d::segmentConstU(
  dtReal const &uu, dtPoint2 const &p0, dtPoint2 const &p1
) const
{
  dtPoint3 p30(uu, p0.x(), p0.y());
  dtPoint3 p31(uu, p1.x(), p0.y());
  dtPoint3 p32(uu, p1.x(), p1.y());
  dtPoint3 p33(uu, p0.x(), p1.y());

  return segment(p30, p31, p32, p33);
}

/**
 *   (w)
 *    A
 *    |
 *            (1)
 *    +--------+
 *    |        |
 *    |        |
 *    |        |
 *    |        |
 *    +--------+ --> (u)
 *   (0)
 */
map2dTo3d *map3dTo3d::segmentConstV(
  dtReal const &vv, dtPoint2 const &p0, dtPoint2 const &p1
) const
{
  dtPoint3 p30(p0.x(), vv, p0.y());
  dtPoint3 p31(p1.x(), vv, p0.y());
  dtPoint3 p32(p1.x(), vv, p1.y());
  dtPoint3 p33(p0.x(), vv, p1.y());

  return segment(p30, p31, p32, p33);
}

/**
 *   (v)
 *    A
 *    |
 *            (1)
 *    +--------+
 *    |        |
 *    |        |
 *    |        |
 *    |        |
 *    +--------+ --> (u)
 *   (0)
 */
map2dTo3d *map3dTo3d::segmentConstW(
  dtReal const &ww, dtPoint2 const &p0, dtPoint2 const &p1
) const
{
  dtPoint3 p30(p0.x(), p0.y(), ww);
  dtPoint3 p31(p1.x(), p0.y(), ww);
  dtPoint3 p32(p1.x(), p1.y(), ww);
  dtPoint3 p33(p0.x(), p1.y(), ww);

  return segment(p30, p31, p32, p33);
}

map2dTo3d *map3dTo3d::segmentConstUPercent(
  dtReal const &uu, dtPoint2 const &p0, dtPoint2 const &p1
) const
{
  dtReal const uP = u_percent(uu);
  dtPoint2 const p0P(v_percent(p0.x()), w_percent(p0.y()));
  dtPoint2 const p1P(v_percent(p1.x()), w_percent(p1.y()));

  return segmentConstU(uP, p0P, p1P);
}

map2dTo3d *map3dTo3d::segmentConstVPercent(
  dtReal const &vv, dtPoint2 const &p0, dtPoint2 const &p1
) const
{
  dtReal const vP = v_percent(vv);
  dtPoint2 const p0P(u_percent(p0.x()), u_percent(p0.y()));
  dtPoint2 const p1P(w_percent(p1.x()), w_percent(p1.y()));

  return segmentConstV(vP, p0P, p1P);
}

map2dTo3d *map3dTo3d::segmentConstWPercent(
  dtReal const &ww, dtPoint2 const &p0, dtPoint2 const &p1
) const
{
  dtReal const wP = w_percent(ww);
  dtPoint2 const p0P(u_percent(p0.x()), v_percent(p0.y()));
  dtPoint2 const p1P(u_percent(p1.x()), v_percent(p1.y()));

  return segmentConstW(wP, p0P, p1P);
}

map2dTo3d *map3dTo3d::segmentConstUPercent(dtReal const &uu) const
{
  return segmentConstU(u_percent(uu));
}

map2dTo3d *map3dTo3d::segmentConstVPercent(dtReal const &vv) const
{
  return segmentConstV(v_percent(vv));
}

map2dTo3d *map3dTo3d::segmentConstWPercent(dtReal const &ww) const
{
  return segmentConstW(w_percent(ww));
}

/**
 *   (v)
 *    A
 *    |
 *
 *   (3)      (2)
 *    +--------+
 *    |        |
 *    |        |
 *    |        |
 *    |        |
 *    +--------+ --> (u)
 *   (0)      (1)
 */
map2dTo3d *map3dTo3d::segment(
  dtPoint3 const &p0, dtPoint3 const &p1, dtPoint3 const &p2, dtPoint3 const &p3
) const
{
  vec3dBiLinearTwoD v3d(p0, p1, p2, p3);
  return new vec3dTwoDInMap3dTo3d(&v3d, this);
}

map2dTo3d *map3dTo3d::segmentPercent(twoDArrayHandling<dtPoint3> const &pp
) const
{
  twoDArrayHandling<dtPoint3> ppUVW(pp.size(0), pp.size(1));
  for (dtInt ii = 0; ii < pp.size(0); ii++)
  {
    for (dtInt jj = 0; jj < pp.size(1); jj++)
    {
      ppUVW[ii][jj] = uvw_percent(pp[ii][jj]);
    }
  }
  return segment(ppUVW);
}

map1dTo3d *
map3dTo3d::segmentPercent(dtPoint3 const &p0, dtPoint3 const &p1) const
{
  return segment(uvw_percent(p0), uvw_percent(p1));
}

map2dTo3d *map3dTo3d::segmentPercent(
  dtPoint3 const &p0, dtPoint3 const &p1, dtPoint3 const &p2, dtPoint3 const &p3
) const
{
  return segment(
    uvw_percent(p0), uvw_percent(p1), uvw_percent(p2), uvw_percent(p3)
  );
}

dtPoint3 map3dTo3d::reparamInVolume(
  dtPoint3 const &ppXYZ, dtVector3 const &uvwExtPercent
) const
{
  gslMinFloatAttr
    md(
      dt__pH(pointGeometryDist)(new pointGeometryDist(ppXYZ, this)),
      // clang-format off
      std::vector<dtPoint3>(
        ::boost::assign::list_of
          (dtPoint3(0.50, 0.50, 0.50))
          (dtPoint3(0.75, 0.50, 0.50))
          (dtPoint3(0.25, 0.50, 0.50))
          (dtPoint3(0.50, 0.75, 0.50))
          (dtPoint3(0.75, 0.75, 0.50))
          (dtPoint3(0.25, 0.75, 0.50))
          (dtPoint3(0.50, 0.25, 0.50))
          (dtPoint3(0.75, 0.25, 0.50))
          (dtPoint3(0.25, 0.25, 0.50))
          (dtPoint3(0.50, 0.50, 0.75))
          (dtPoint3(0.75, 0.50, 0.75))
          (dtPoint3(0.25, 0.50, 0.75))
          (dtPoint3(0.50, 0.75, 0.75))
          (dtPoint3(0.75, 0.75, 0.75))
          (dtPoint3(0.25, 0.75, 0.75))
          (dtPoint3(0.50, 0.25, 0.75))
          (dtPoint3(0.75, 0.25, 0.75))
          (dtPoint3(0.25, 0.25, 0.75))
          (dtPoint3(0.50, 0.50, 0.25))
          (dtPoint3(0.75, 0.50, 0.25))
          (dtPoint3(0.25, 0.50, 0.25))
          (dtPoint3(0.50, 0.75, 0.25))
          (dtPoint3(0.75, 0.75, 0.25))
          (dtPoint3(0.25, 0.75, 0.25))
          (dtPoint3(0.50, 0.25, 0.25))
          (dtPoint3(0.75, 0.25, 0.25))
          (dtPoint3(0.25, 0.25, 0.25))
      ),
      // clang-format on
      dtPoint3(0.001, 0.001, 0.001),
      staticPropertiesHandler::getInstance()->getOptionFloat("xyz_resolution"),
      1000
    );
  md.perform();
  dt__throwIf(!md.converged(), reparamInVolume());
  return uvw_percent(dtPoint3(md.result()[0], md.result()[1], md.result()[2]));
}

dt__C_addCloneForpVH(map3dTo3d);
} // namespace dtOO
