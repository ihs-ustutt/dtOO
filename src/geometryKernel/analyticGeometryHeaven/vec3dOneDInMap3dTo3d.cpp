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

#include "vec3dOneDInMap3dTo3d.h"

#include "map1dTo3d.h"
#include "map1dTo3dTransformed.h"
#include "map3dTo3d.h"
#include <analyticFunctionHeaven/vec3dCurveOneD.h>
#include <analyticFunctionHeaven/vec3dOneD.h>
#include <discrete3dPoints.h>
#include <discrete3dVector.h>
#include <dtTransformerHeaven/dtTransformer.h>
#include <geometryEngine/dtCurve.h>
#include <interfaceHeaven/ptrVectorHandling.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

namespace dtOO {
vec3dOneDInMap3dTo3d::vec3dOneDInMap3dTo3d() : map1dTo3d() {}

vec3dOneDInMap3dTo3d::vec3dOneDInMap3dTo3d(const vec3dOneDInMap3dTo3d &orig)
  : map1dTo3d(orig)
{
  _v1d.reset(orig._v1d->clone());
  _m3d.reset(orig._m3d->clone());
  _percentF = orig._percentF;
}

vec3dOneDInMap3dTo3d::vec3dOneDInMap3dTo3d(
  vec3dOneD const *const v1d, map3dTo3d const *const m3d, bool percentF
)
{
  _v1d.reset(v1d->clone());
  _m3d.reset(m3d->clone());
  _percentF = percentF;
}

vec3dOneDInMap3dTo3d::~vec3dOneDInMap3dTo3d() {}

dtPoint3 vec3dOneDInMap3dTo3d::getPoint(dtReal const &uu) const
{
  dtPoint3 pUVW = _v1d->YdtPoint3(uu);
  if (!_percentF)
  {
    return _m3d->getPoint(pUVW.x(), pUVW.y(), pUVW.z());
  }
  else
  {
    return _m3d->getPointPercent(pUVW.x(), pUVW.y(), pUVW.z());
  }
}

bool vec3dOneDInMap3dTo3d::isClosed(dtInt const &dir) const
{
  return _v1d->closed(dir);
}

dtReal vec3dOneDInMap3dTo3d::getMin(dtInt const &dir) const
{
  return _v1d->xMin(dir);
}

dtReal vec3dOneDInMap3dTo3d::getMax(dtInt const &dir) const
{
  return _v1d->xMax(dir);
}

vec3dOneDInMap3dTo3d *vec3dOneDInMap3dTo3d::create(void) const
{
  return new vec3dOneDInMap3dTo3d();
}

vec3dOneDInMap3dTo3d *vec3dOneDInMap3dTo3d::clone(void) const
{
  return new vec3dOneDInMap3dTo3d(*this);
}

vec3dOneDInMap3dTo3d *
vec3dOneDInMap3dTo3d::cloneTransformed(dtTransformer const *const dtT) const
{
  return new map1dTo3dTransformed<vec3dOneDInMap3dTo3d>(*this, dtT);
}

vec3dOneD const *vec3dOneDInMap3dTo3d::refToVec3dOneD(void) const
{
  return _v1d.get();
}

map3dTo3d const *vec3dOneDInMap3dTo3d::refToMap3dTo3d(void) const
{
  return _m3d.get();
}

vectorHandling<renderInterface *> vec3dOneDInMap3dTo3d::getExtRender(void) const
{
  vectorHandling<dtVector3> vv;
  vectorHandling<dtPoint3> vp;
  vectorHandling<dtPoint3> pp;

  //
  // get spline direction
  //
  dtPoint3 startPoint = getPointPercent(0.05);
  dtPoint3 topPoint = getPointPercent(0.1);
  dtVector3 uu = topPoint - startPoint;

  //
  // add direction vector to container
  //
  vv.push_back(uu);
  vp.push_back(startPoint);

  vectorHandling<renderInterface *> retVec;
  retVec.push_back(new discrete3dVector(vv, vp));

  //
  // get control points
  //
  vec3dCurveOneD const *const v3dC1d =
    vec3dCurveOneD::ConstDownCast(_v1d.get());
  if (v3dC1d)
  {
    dtCurve const *const dtC = v3dC1d->ptrDtCurve();
    int nPointsU = dtC->nControlPoints();
    dtInt renderMaxPoints =
      staticPropertiesHandler::getInstance()->getOptionInt("render_max_nPoints"
      );
    if (nPointsU <= renderMaxPoints)
    {
      for (int ii = 0; ii < nPointsU; ii++)
      {
        pp.push_back(_m3d->getPoint(dtC->controlPoint(ii)));
      }
      // retVec.push_back( new discrete3dPoints(pp) );
    }
    else
    {
      dt__warning(
        extRender(),
        << "Number of control points above option > render_max_nPoints <."
      );
    }
  }

  if (pp.empty())
  {
    pp.push_back(this->getPointPercent(0.));
    pp.push_back(this->getPointPercent(1.));
  }
  retVec.push_back(new discrete3dPoints(pp));
  return retVec;
}
} // namespace dtOO
