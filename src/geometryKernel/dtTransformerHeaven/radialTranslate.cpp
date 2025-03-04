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

#include "radialTranslate.h"

#include <logMe/logMe.h>
#include <progHelper.h>
#include <xmlHeaven/dtXmlParserBase.h>

#include "dtTransformerFactory.h"
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map1dTo3dTransformed.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3dTransformed.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3dTransformed.h>

namespace dtOO {
bool radialTranslate::_registrated = dtTransformerFactory::registrate(
  dt__tmpPtr(radialTranslate, new radialTranslate())
);

radialTranslate::radialTranslate() : dtTransformerInvThreeD()
{
  _v3 = dtVector3(0, 0, 0);
  _distance = 0.;
}

radialTranslate::radialTranslate(radialTranslate const &orig)
  : dtTransformerInvThreeD(orig)
{
  _v3 = orig._v3;
  _distance = orig._distance;
}

radialTranslate::~radialTranslate() {}

dtTransformerInvThreeD *radialTranslate::clone(void) const
{
  return new radialTranslate(*this);
}

dtTransformerInvThreeD *radialTranslate::create(void) const
{
  return new radialTranslate();
}

std::vector<dtPoint3>
radialTranslate::apply(std::vector<dtPoint3> const *const toTrans) const
{
  std::vector<dtPoint3> ret(toTrans->size());

  dt__forAllIndex(*toTrans, ii)
  {
    dtPoint3 P_0 = toTrans->at(ii);
    dtVector3 RR(P_0.x(), P_0.y(), P_0.z());
    dtVector3 RR_z = dtLinearAlgebra::dotProduct(RR, _v3) * _v3;
    dtPoint3 P_1 = P_0 + dtLinearAlgebra::normalize(RR - RR_z) * _distance;
    ret[ii] = P_1;
  }

  return ret;
}

std::vector<dtVector3>
radialTranslate::apply(std::vector<dtVector3> const *const toTrans) const
{
  std::vector<dtVector3> ret(toTrans->size());

  dt__forAllIndex(*toTrans, ii) { ret[ii] = toTrans->at(ii); }

  return ret;
}

std::vector<dtPoint3>
radialTranslate::retract(std::vector<dtPoint3> const *const toRetract) const
{
  std::vector<dtPoint3> ret(toRetract->size());

  dt__forAllIndex(*toRetract, ii)
  {
    dtPoint3 P_0 = toRetract->at(ii);
    dtVector3 RR(P_0.x(), P_0.y(), P_0.z());
    dtVector3 RR_z = dtLinearAlgebra::dotProduct(RR, _v3) * _v3;
    dtPoint3 P_1 = P_0 - dtLinearAlgebra::normalize(RR - RR_z) * _distance;
    ret[ii] = P_1;
  }

  return ret;
}

std::vector<dtVector3>
radialTranslate::retract(std::vector<dtVector3> const *const toTrans) const
{
  std::vector<dtVector3> ret(toTrans->size());

  dt__forAllIndex(*toTrans, ii) { ret[ii] = toTrans->at(ii); }

  return ret;
}

lvH_analyticGeometry
radialTranslate::apply(lvH_analyticGeometry const *const aGeoVecP) const
{
  lvH_analyticGeometry retAGeo;

  dt__forAllRefAuto(*aGeoVecP, aG)
  {
    //
    // clone and cast analyticGeometry
    //
    map1dTo3d const *const m1d = map1dTo3d::ConstDownCast(aG);
    map2dTo3d const *const m2d = map2dTo3d::ConstDownCast(aG);
    map3dTo3d const *const m3d = map3dTo3d::ConstDownCast(aG);

    if (m1d)
      retAGeo.push_back(m1d->cloneTransformed(this));
    if (m2d)
      retAGeo.push_back(m2d->cloneTransformed(this));
    if (m3d)
      retAGeo.push_back(m3d->cloneTransformed(this));
  }
  return retAGeo;
}

bool radialTranslate::isNecessary(void) const { return true; }

void radialTranslate::init(
  ::QDomElement const *tE,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG
)
{
  dtTransformer::init(tE, bC, cV, aF, aG);

  ::QDomElement v3El = dtXmlParserBase::getChild("Vector_3", *tE);
  _v3 = dtXmlParserBase::getDtVector3(&v3El, bC, cV, aF, aG);
  _distance =
    dtXmlParserBase::getAttributeFloatMuParse("distance", *tE, cV, aF, aG);
}
} // namespace dtOO
