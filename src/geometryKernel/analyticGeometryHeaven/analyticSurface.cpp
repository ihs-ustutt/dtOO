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

#include "analyticSurface.h"

#include "analyticCurve.h"
#include "map1dTo3d.h"
#include "map2dTo3dTransformed.h"
#include "vec2dOneDInMap2dTo3d.h"
#include <analyticFunctionHeaven/vec2dCurve2dOneD.h>
#include <discrete3dPoints.h>
#include <discrete3dVector.h>
#include <dtTransformerHeaven/dtTransformer.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtCurve2d.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/geoBuilder/rectangularTrimmedSurface_uvBounds.h>
#include <geometryEngine/geoBuilder/trimmedCurve2d_twoPointsConnectConstructOCC.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

namespace dtOO {
analyticSurface::analyticSurface() : map2dTo3d() {}

analyticSurface::analyticSurface(analyticSurface const &orig) : map2dTo3d(orig)
{
  _dtS.reset(orig._dtS->clone());
}

analyticSurface::analyticSurface(dtSurface const *const surface) : map2dTo3d()
{
  _dtS.reset(surface->clone());
}

analyticSurface *analyticSurface::clone(void) const
{
  return new analyticSurface(*this);
}

analyticSurface *analyticSurface::create(void) const
{
  return new analyticSurface();
}

analyticSurface *
analyticSurface::cloneTransformed(dtTransformer const *const dtT) const
{
  return new map2dTo3dTransformed<analyticSurface>(*this, dtT);
}

analyticSurface::~analyticSurface() {}

dtPoint3 analyticSurface::getPoint(dtReal const &uu, dtReal const &vv) const
{
  //    dt__warnIfWithMessage(uu<getUMin(), getPoint(), << uu << " < " <<
  //    getUMin());
  //		dt__warnIfWithMessage(vv<getVMin(), getPoint(), << vv << " < "
  //<< getVMin());
  //    dt__warnIfWithMessage(uu>getUMax(), getPoint(), << uu << " > " <<
  //    getUMax());
  //		dt__warnIfWithMessage(vv>getVMax(), getPoint(), << vv << " > "
  //<< getVMax());

  return _dtS->point(uu, vv);
}

vectorHandling<renderInterface *> analyticSurface::getExtRender(void) const
{
  vectorHandling<dtVector3> vecV;
  vectorHandling<dtPoint3> vecP;
  vectorHandling<dtVector3> norV;
  vectorHandling<dtPoint3> norP;
  vectorHandling<dtPoint3> pp;
  //
  // get surface directions
  //
  dtPoint3 startPointU = map2dTo3d::getPointPercent(
    0.05, 0.
  ); // getDtSislSurf()->getPointPercent3d(0.05, 0.);
  dtPoint3 topPointU = map2dTo3d::getPointPercent(
    0.1, 0.
  ); // getDtSislSurf()->getPointPercent3d(0.1, 0.);
  dtPoint3 startPointV = map2dTo3d::getPointPercent(
    0., 0.05
  ); // getDtSislSurf()->getPointPercent3d(0., 0.05);
  dtPoint3 topPointV = map2dTo3d::getPointPercent(
    0., 0.10
  ); // getDtSislSurf()->getPointPercent3d(0., 0.10);
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
  norV.push_back(map2dTo3d::normalPercent(.25, .25));
  norP.push_back(map2dTo3d::getPointPercent(.25, .25));
  norV.push_back(map2dTo3d::normalPercent(.25, .75));
  norP.push_back(map2dTo3d::getPointPercent(.25, .75));
  norV.push_back(map2dTo3d::normalPercent(.75, .25));
  norP.push_back(map2dTo3d::getPointPercent(.75, .25));
  norV.push_back(map2dTo3d::normalPercent(.75, .75));
  norP.push_back(map2dTo3d::getPointPercent(.75, .75));

  //
  // get control points
  //
  dtInt numPointsU = _dtS->nControlPoints(0);
  int numPointsV = _dtS->nControlPoints(1);
  for (int ii = 0; ii < numPointsU; ii++)
  {
    for (int jj = 0; jj < numPointsV; jj++)
    {
      pp.push_back(_dtS->controlPoint(ii, jj));
    }
  }
  pp.push_back(map2dTo3d::getPointPercent(0., .5));
  pp.push_back(map2dTo3d::getPointPercent(.5, 0.));

  vectorHandling<renderInterface *> retVec(3);
  retVec[0] = new discrete3dPoints(pp);
  retVec[1] = new discrete3dVector(norV, norP);
  retVec[2] = new discrete3dVector(vecV, vecP);

  return retVec;
}

dtSurface *analyticSurface::ptrDtSurface(void) const { return _dtS.get(); }

dtSurface const *analyticSurface::ptrConstDtSurface(void) const
{
  return _dtS.get();
}

void analyticSurface::offsetNormal(dtReal const nn) { _dtS->offsetNormal(nn); }

bool analyticSurface::isClosed(dtInt const &dir) const
{
  return _dtS->closed(dir);
}

dtReal analyticSurface::getMin(dtInt const &dir) const
{
  return _dtS->minPara(dir);
}

dtReal analyticSurface::getMax(dtInt const &dir) const
{
  return _dtS->maxPara(dir);
}

dtVector3 analyticSurface::normal(dtReal const &uu, dtReal const &vv) const
{
  return _dtS->normal(uu, vv);
}

std::vector<dtVector3>
analyticSurface::firstDer(dtReal const &uu, dtReal const &vv) const
{
  std::vector<dtVector3> dd(2);
  dd[0] = _dtS->firstDerU(uu, vv);
  dd[1] = _dtS->firstDerV(uu, vv);

  return dd;
}

std::vector<dtVector3>
analyticSurface::secondDer(dtReal const &uu, dtReal const &vv) const
{
  std::vector<dtVector3> dd(3);
  dd[0] = _dtS->secondDerUU(uu, vv);
  dd[1] = _dtS->secondDerUV(uu, vv);
  dd[2] = _dtS->secondDerVV(uu, vv);

  return dd;
}

dtPoint2 analyticSurface::reparamOnFace(dtPoint3 const &ppXYZ) const
{
  dtPoint2 ppUV = _dtS->reparam(ppXYZ);
  dtPoint3 ppXYZReparam = getPoint(ppUV.x(), ppUV.y());

  dt__throwIf(
    !analyticGeometry::inXYZTolerance(ppXYZ, ppXYZReparam), reparamOnFace()
  );

  return ppUV;
}

map1dTo3d *analyticSurface::segmentConstU(
  dtReal const &uu, dtReal const &p0, dtReal const &p1
) const
{
  ptrHandling<dtCurve> cc(_dtS->segmentConstU(uu, p0, p1));
  return new analyticCurve(cc.get());
}

map1dTo3d *analyticSurface::segmentConstV(
  dtReal const &vv, dtReal const &p0, dtReal const &p1
) const
{
  ptrHandling<dtCurve> cc(_dtS->segmentConstV(vv, p0, p1));
  return new analyticCurve(cc.get());
}

map2dTo3d *
analyticSurface::segmentRectangle(dtPoint2 const &p0, dtPoint2 const &p1) const
{
  ptrHandling<dtSurface> ss(
    rectangularTrimmedSurface_uvBounds(_dtS.get(), p0, p1).result()
  );
  return new analyticSurface(ss.get());
}

std::string analyticSurface::dumpToString(void) const
{
  std::stringstream ss;

  ss << dt__dumpToString(
    << "_dtS = " << _dtS->virtualClassName() << std::endl
    << _dtS->dumpToString()
  );

  return ss.str();
}
} // namespace dtOO
