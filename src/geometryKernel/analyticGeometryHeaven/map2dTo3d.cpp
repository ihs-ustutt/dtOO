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

#include "map2dTo3d.h"

#include "map1dTo3d.h"
#include "vec2dOneDInMap2dTo3d.h"
#include "vec2dTwoDInMap2dTo3d.h"
#include <analyticFunctionHeaven/vec2dBiLinearTwoD.h>
#include <analyticFunctionHeaven/vec2dCurve2dOneD.h>
#include <analyticGeometryHeaven/aGBuilder/uv_map2dTo3dClosestPointToPoint.h>
#include <discrete3dPoints.h>
#include <discrete3dVector.h>
#include <geometryEngine/dtCurve2d.h>
#include <geometryEngine/geoBuilder/trimmedCurve2d_twoPointsConnectConstructOCC.h>
#include <interfaceHeaven/calculationTypeHandling.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <interfaceHeaven/twoDArrayHandling.h>
#include <logMe/dtMacros.h>
#include <logMe/logContainer.h>
#include <logMe/logMe.h>
#include <solid3dSurface.h>

#include <attributionHeaven/pointGeometryDist.h>
#include <boost/assign/list_of.hpp>
#include <gslMinFloatAttr.h>

namespace dtOO {
dtReal map2dTo3d::_deltaPer =
  staticPropertiesHandler::getInstance()->getOptionFloat("map2dTo3d_deltaPer");

map2dTo3d::map2dTo3d() : analyticGeometry() {}

map2dTo3d::map2dTo3d(const map2dTo3d &orig) : analyticGeometry(orig) {}

map2dTo3d::~map2dTo3d() {}

dtInt map2dTo3d::dim(void) const { return 2; }

dtPoint3 map2dTo3d::getPoint(dtReal const *const uvw) const
{
  return getPoint(uvw[0], uvw[1]);
}

dtPoint3 map2dTo3d::getPoint(dtPoint2 const &pUV) const
{
  return getPoint(pUV.x(), pUV.y());
}

dtPoint3 map2dTo3d::getPoint(dtPoint2 const *const pUV) const
{
  return getPoint(*pUV);
}

bool map2dTo3d::isClosedU(void) const { return isClosed(0); }

bool map2dTo3d::isClosedV(void) const { return isClosed(1); }

dtReal map2dTo3d::getUMin(void) const { return getMin(0); }

dtReal map2dTo3d::getUMax(void) const { return getMax(0); }

dtReal map2dTo3d::getVMin(void) const { return getMin(1); }

dtReal map2dTo3d::getVMax(void) const { return getMax(1); }

bool map2dTo3d::inRange(dtPoint2 const &pUV) const
{
  if (pUV.x() > getUMax())
    return false;
  if (pUV.x() < getUMin())
    return false;
  if (pUV.y() > getVMax())
    return false;
  if (pUV.y() < getVMin())
    return false;
  return true;
}

dtPoint3 map2dTo3d::getPointPercent(dtReal const &uu, dtReal const &vv) const
{
  return getPoint(u_percent(uu), v_percent(vv));
}

dtPoint3 map2dTo3d::getPointPercent(dtPoint2 const &pUV) const
{
  return getPointPercent(pUV.x(), pUV.y());
}

vectorHandling<renderInterface *> map2dTo3d::getRender(void) const
{
  int renderResU = analyticGeometry::getRenderResolution(0);
  int renderResV = analyticGeometry::getRenderResolution(1);

  //
  // calculate interval to create points
  //
  dtReal intervalFirst = (getUMax() - getUMin()) / (renderResU - 1.);
  dtReal intervalSecond = (getVMax() - getVMin()) / (renderResV - 1.);

  twoDArrayHandling<dtPoint3> surfacePoints(renderResU, renderResV);
  for (int jj = 0; jj < renderResU; jj++)
  {
    for (int ii = 0; ii < renderResV; ii++)
    {
      dtReal uu;
      dtReal vv;
      if (jj == (renderResU - 1))
      {
        uu = getUMax();
      }
      else
      {
        uu = (dtReal)(getUMin() + jj * intervalFirst);
      }
      if (ii == (renderResV - 1))
      {
        vv = getVMax();
      }
      else
      {
        vv = (dtReal)(getVMin() + ii * intervalSecond);
      }
      surfacePoints[jj][ii] = getPoint(uu, vv);
    }
  }

  vectorHandling<renderInterface *> rV;
  rV.push_back(new solid3dSurface(surfacePoints));

  //
  // get surface directions
  //
  vectorHandling<dtVector3> vecV;
  vectorHandling<dtPoint3> vecP;
  dtPoint3 startPointU = map2dTo3d::getPointPercent(0.05, 0.);
  dtPoint3 topPointU = map2dTo3d::getPointPercent(0.1, 0.);
  dtPoint3 startPointV = map2dTo3d::getPointPercent(0., 0.05);
  dtPoint3 topPointV = map2dTo3d::getPointPercent(0., 0.10);
  dtVector3 uu = topPointU - startPointU;
  dtVector3 vv = topPointV - startPointV;
  //
  // add direction to vector container
  //
  vecV.push_back(uu);
  vecP.push_back(startPointU);
  vecV.push_back(vv);
  vecP.push_back(startPointV);
  vecV.push_back(vv);
  vecP.push_back(topPointV);

  rV.push_back(new discrete3dVector(vecV, vecP));
  return rV;
}

vectorHandling<renderInterface *> map2dTo3d::getExtRender(void) const
{
  vectorHandling<dtVector3> norV;
  vectorHandling<dtPoint3> norP;

  //
  // add normals
  //
  norV.push_back(map2dTo3d::normalPercent(.25, .25));
  norP.push_back(map2dTo3d::getPointPercent(.25, .25));
  norV.push_back(map2dTo3d::normalPercent(.25, .75));
  norP.push_back(map2dTo3d::getPointPercent(.25, .75));
  norV.push_back(map2dTo3d::normalPercent(.75, .25));
  norP.push_back(map2dTo3d::getPointPercent(.75, .25));
  norV.push_back(map2dTo3d::normalPercent(.75, .75));
  norP.push_back(map2dTo3d::getPointPercent(.75, .75));

  vectorHandling<renderInterface *> retVec(1);
  retVec[0] = new discrete3dVector(norV, norP);
  return retVec;
}

dtInt map2dTo3d::getRenderResolutionU(void) const
{
  return analyticGeometry::getRenderResolution(0);
}

dtInt map2dTo3d::getRenderResolutionV(void) const
{
  return analyticGeometry::getRenderResolution(1);
}

dtVector3
map2dTo3d::getPointPercentVector(dtReal const &uu, dtReal const &vv) const
{
  return dtLinearAlgebra::toDtVector3(getPointPercent(uu, vv));
}

dtVector3 map2dTo3d::normal(dtReal const &uu, dtReal const &vv) const
{
  dtVector3 vec(
    dtLinearAlgebra::crossProduct(firstDerU(uu, vv), firstDerV(uu, vv))
  );
  return (1. / sqrt(vec.squared_length())) * vec;
}

dtVector3 map2dTo3d::normal(dtPoint2 const &pp) const
{
  return normal(pp.x(), pp.y());
}

dtPoint2 map2dTo3d::reparamOnFace(dtPoint3 const &ppXYZ) const
{
  gslMinFloatAttr md(
    dt__pH(pointGeometryDist)(new pointGeometryDist(ppXYZ, this)),
    // clang-format off
    std::vector<dtPoint2>(
      ::boost::assign::list_of
        (dtPoint2(0.50, 0.50))
        (dtPoint2(0.75, 0.50))
        (dtPoint2(0.25, 0.50))
        (dtPoint2(0.50, 0.75))
        (dtPoint2(0.75, 0.75))
        (dtPoint2(0.25, 0.75))
        (dtPoint2(0.50, 0.25))
        (dtPoint2(0.75, 0.25))
        (dtPoint2(0.25, 0.25))
    ),
    // clang-format on
    dtPoint2(0.001, 0.001),
    staticPropertiesHandler::getInstance()->getOptionFloat("xyz_resolution"),
    1000
  );
  md.perform();
  dt__throwIf(!md.converged(), reparamOnFace());
  return uv_percent(dtPoint2(md.result()[0], md.result()[1]));
}

dtPoint2 map2dTo3d::approxOnFace(dtPoint3 const &ppXYZ) const
{
  return uv_map2dTo3dClosestPointToPoint(this, ppXYZ).result();
}

dtVector3 map2dTo3d::normalPercent(dtReal const &uu, dtReal const &vv) const
{
  return normal(u_percent(uu), v_percent(vv));
}

std::vector<dtVector3>
map2dTo3d::firstDer(dtReal const &uu, dtReal const &vv) const
{
  //
  // dU
  //
  dtReal uP = percent_u(uu);
  dtReal vP = percent_v(vv);
  dtReal const deltaPerInv = 1. - _deltaPer;
  std::vector<dtVector3> dd(2);

  if (uP < _deltaPer)
  {
    dd[0] = (getPointPercent(_deltaPer, vP) - getPointPercent(0., vP)) /
            (u_percent(_deltaPer) - u_percent(0.));
  }
  else if ((uP >= _deltaPer) && (uP <= deltaPerInv))
  {
    dd[0] = (getPointPercent(uP + _deltaPer, vP) -
             getPointPercent(uP - _deltaPer, vP)) /
            (u_percent(uP + _deltaPer) - u_percent(uP - _deltaPer));
  }
  else if (uP > deltaPerInv)
  {
    dd[0] = (getPointPercent(1., vP) - getPointPercent(1. - _deltaPer, vP)) /
            (u_percent(1.) - u_percent(deltaPerInv));
  }

  //
  // dV
  //
  if (vP < _deltaPer)
  {
    dd[1] = (getPointPercent(uP, _deltaPer) - getPointPercent(uP, 0.)) /
            (v_percent(_deltaPer) - v_percent(0.));
  }
  else if ((vP >= _deltaPer) && (vP <= deltaPerInv))
  {
    dd[1] = (getPointPercent(uP, vP + _deltaPer) -
             getPointPercent(uP, vP - _deltaPer)) /
            (v_percent(vP + _deltaPer) - v_percent(vP - _deltaPer));
  }
  else if (vP > deltaPerInv)
  {
    dd[1] = (getPointPercent(uP, 1.) - getPointPercent(uP, 1. - _deltaPer)) /
            (v_percent(1.) - v_percent(deltaPerInv));
  }

  return dd;
}

std::vector<dtVector3> map2dTo3d::firstDer(dtPoint2 const &pp) const
{
  return firstDer(pp.x(), pp.y());
}

dtVector3 map2dTo3d::firstDerU(dtReal const &uu, dtReal const &vv) const
{
  return firstDer(uu, vv)[0];
}

dtVector3 map2dTo3d::firstDerV(dtReal const &uu, dtReal const &vv) const
{
  return firstDer(uu, vv)[1];
}

std::vector<dtVector3>
map2dTo3d::secondDer(dtReal const &uu, dtReal const &vv) const
{
  dtReal uP = percent_u(uu);
  dtReal vP = percent_v(vv);
  dtReal const deltaPerInv = 1. - _deltaPer;
  std::vector<dtVector3> dd(3);

  //
  // dUU
  //
  if (uP < _deltaPer)
  {
    dd[0] = (firstDerU(_deltaPer, vP) - firstDerU(0., vP)) /
            (u_percent(_deltaPer) - u_percent(0.));
  }
  else if ((uP >= _deltaPer) && (uP <= deltaPerInv))
  {
    dd[0] = (firstDerU(uP + _deltaPer, vP) - firstDerU(uP - _deltaPer, vP)) /
            (u_percent(uP + _deltaPer) - u_percent(uP - _deltaPer));
  }
  else if (uP > deltaPerInv)
  {
    dd[0] = (firstDerU(1., vP) - firstDerU(1. - _deltaPer, vP)) /
            (u_percent(1.) - u_percent(deltaPerInv));
  }

  //
  // ddUV
  //
  if (vP < _deltaPer)
  {
    dd[1] = (firstDerV(uP, _deltaPer) - firstDerV(uP, 0.)) /
            (u_percent(_deltaPer) - u_percent(0.));
  }
  else if ((vP >= _deltaPer) && (vP <= deltaPerInv))
  {
    dd[1] = (firstDerV(uP, vP + _deltaPer) - firstDerV(uP, vP - _deltaPer)) /
            (u_percent(uP + _deltaPer) - u_percent(uP - _deltaPer));
  }
  else if (vP > deltaPerInv)
  {
    dd[1] = (firstDerV(uP, 1.) - firstDerV(uP, 1. - _deltaPer)) /
            (u_percent(1.) - u_percent(deltaPerInv));
  }

  //
  // dVV
  //
  if (vP < _deltaPer)
  {
    dd[2] = (firstDerV(uP, _deltaPer) - firstDerV(uP, 0.)) /
            (v_percent(_deltaPer) - v_percent(0.));
  }
  else if ((vP >= _deltaPer) && (vP <= deltaPerInv))
  {
    dd[2] = (firstDerV(uP, vP + _deltaPer) - firstDerU(uP, vP - _deltaPer)) /
            (v_percent(vP + _deltaPer) - v_percent(vP - _deltaPer));
  }
  else if (vP > deltaPerInv)
  {
    dd[2] = (firstDerV(uP, 1.) - firstDerV(uP, 1. - _deltaPer)) /
            (v_percent(1.) - v_percent(deltaPerInv));
  }

  return dd;
}

dtVector3 map2dTo3d::secondDerUU(dtReal const &uu, dtReal const &vv) const
{
  return secondDer(uu, vv)[0];
}

dtVector3 map2dTo3d::secondDerVV(dtReal const &uu, dtReal const &vv) const
{
  return secondDer(uu, vv)[2];
}

dtVector3 map2dTo3d::secondDerUV(dtReal const &uu, dtReal const &vv) const
{
  return secondDer(uu, vv)[1];
}

dtMatrix map2dTo3d::jacobi(dtReal const &uu, dtReal const &vv) const
{
  return dtLinearAlgebra::createMatrixGiveColumns(firstDer(uu, vv));
}

dtMatrix map2dTo3d::jacobi(dtPoint2 const &pp) const
{
  return jacobi(pp.x(), pp.y());
}

dtPoint2 map2dTo3d::reparamPercentOnFace(dtPoint3 const &ppXYZ) const
{
  dtPoint2 ppUV = reparamOnFace(ppXYZ);

  return percent_uv(ppUV);
}

dtPoint2 map2dTo3d::approxPercentOnFace(dtPoint3 const &ppXYZ) const
{
  dtPoint2 ppUV = approxOnFace(ppXYZ);

  return percent_uv(ppUV);
}

dtPoint2 map2dTo3d::uv_percent(dtPoint2 const &pUV) const
{
  return dtPoint2(u_percent(pUV.x()), v_percent(pUV.y()));
}

dtPoint2 map2dTo3d::uv_percent(dtReal const &uu, dtReal const &vv) const
{
  return dtPoint2(u_percent(uu), v_percent(vv));
}

dtReal map2dTo3d::u_percent(dtReal const &uu) const
{
  return floatHandling::boundToRange(
    getUMin() + (getUMax() - getUMin()) * uu, getUMin(), getUMax()
  );
}

dtReal map2dTo3d::v_percent(dtReal const &vv) const
{
  return floatHandling::boundToRange(
    getVMin() + (getVMax() - getVMin()) * vv, getVMin(), getVMax()
  );
}

dtPoint2 map2dTo3d::percent_uv(dtPoint2 const &pUV) const
{
  return dtPoint2(percent_u(pUV.x()), percent_v(pUV.y()));
}

dtReal map2dTo3d::percent_u(dtReal const &uu) const
{
  return floatHandling::boundToRange(
    (uu - getUMin()) / (getUMax() - getUMin()), 0., 1.
  );
}

dtReal map2dTo3d::percent_v(dtReal const &vv) const
{
  return floatHandling::boundToRange(
    (vv - getVMin()) / (getVMax() - getVMin()), 0., 1.
  );
}

map1dTo3d *map2dTo3d::segment(dtPoint2 const &p0, dtPoint2 const &p1) const
{
  ptrHandling<dtCurve2d> dtC2d(
    trimmedCurve2d_twoPointsConnectConstructOCC(p0, p1).result()
  );
  vec2dCurve2dOneD v2d1d(dtC2d.get());
  return new vec2dOneDInMap2dTo3d(&v2d1d, this);
}

map1dTo3d *map2dTo3d::segmentConstU(
  dtReal const &uu, dtReal const &p0, dtReal const &p1
) const
{
  dtPoint2 p20(uu, p0);
  dtPoint2 p21(uu, p1);

  return segment(p20, p21);
}

map1dTo3d *map2dTo3d::segmentConstV(
  dtReal const &vv, dtReal const &p0, dtReal const &p1
) const
{
  dtPoint2 p20(p0, vv);
  dtPoint2 p21(p1, vv);

  return segment(p20, p21);
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
map2dTo3d *map2dTo3d::segment(
  dtPoint2 const &p0, dtPoint2 const &p1, dtPoint2 const &p2, dtPoint2 const &p3
) const
{
  return new vec2dTwoDInMap2dTo3d(
    dt__tmpPtr(vec2dTwoD, new vec2dBiLinearTwoD(p0, p1, p2, p3)), this
  );
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
map2dTo3d *
map2dTo3d::segmentRectangle(dtPoint2 const &p0, dtPoint2 const &p1) const
{
  return segment(p0, dtPoint2(p1.x(), p0.y()), p1, dtPoint2(p0.x(), p1.y()));
}

map1dTo3d *map2dTo3d::segmentConstU(dtReal const &uu) const
{
  return segmentConstU(uu, getVMin(), getVMax());
}

map1dTo3d *map2dTo3d::segmentConstV(dtReal const &vv) const
{
  return segmentConstV(vv, getUMin(), getUMax());
}

map1dTo3d *map2dTo3d::segmentConstUPercent(
  dtReal const &uu, dtReal const &p0, dtReal const &p1
) const
{
  return segmentConstU(u_percent(uu), v_percent(p0), v_percent(p1));
}

map1dTo3d *map2dTo3d::segmentConstVPercent(
  dtReal const &vv, dtReal const &p0, dtReal const &p1
) const
{
  return segmentConstV(v_percent(vv), u_percent(p0), u_percent(p1));
}

map1dTo3d *map2dTo3d::segmentConstUPercent(dtReal const &uu) const
{
  return segmentConstU(u_percent(uu));
}

map1dTo3d *map2dTo3d::segmentConstVPercent(dtReal const &vv) const
{
  return segmentConstV(v_percent(vv));
}

map1dTo3d *
map2dTo3d::segmentPercent(dtPoint2 const &p0, dtPoint2 const &p1) const
{
  return segment(uv_percent(p0), uv_percent(p1));
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
map2dTo3d *map2dTo3d::segmentPercent(
  dtPoint2 const &p0, dtPoint2 const &p1, dtPoint2 const &p2, dtPoint2 const &p3
) const
{
  return segment(
    uv_percent(p0), uv_percent(p1), uv_percent(p2), uv_percent(p3)
  );
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
map2dTo3d *
map2dTo3d::segmentRectanglePercent(dtPoint2 const &p0, dtPoint2 const &p1) const
{
  return segmentRectangle(uv_percent(p0), uv_percent(p1));
}

dtPoint2 map2dTo3d::operator%(const dtPoint2 &percent) const
{
  return dtPoint2(this->uv_percent(percent));
}

map2dTo3d *new_clone(map2dTo3d const &m2d) { return m2d.clone(); }
} // namespace dtOO
