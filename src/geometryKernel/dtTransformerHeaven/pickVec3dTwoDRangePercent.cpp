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

#include "pickVec3dTwoDRangePercent.h"

#include "dtTransformerFactory.h"
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/vec3dCurveOneD.h>
#include <analyticFunctionHeaven/vec3dSurfaceTwoD.h>
#include <analyticFunctionHeaven/vec3dTwoD.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtSurface.h>
#include <logMe/logMe.h>

namespace dtOO {
bool pickVec3dTwoDRangePercent::_registrated = dtTransformerFactory::registrate(
  dt__tmpPtr(pickVec3dTwoDRangePercent, new pickVec3dTwoDRangePercent())
);

pickVec3dTwoDRangePercent::pickVec3dTwoDRangePercent() : dtTransformer() {}

pickVec3dTwoDRangePercent::pickVec3dTwoDRangePercent(
  const pickVec3dTwoDRangePercent &orig
)
  : dtTransformer(orig)
{
}

pickVec3dTwoDRangePercent::pickVec3dTwoDRangePercent(jsonPrimitive const &jE)
  : dtTransformer(jE)
{
  this->jInit(jE, NULL, NULL, NULL, NULL);
}

pickVec3dTwoDRangePercent::~pickVec3dTwoDRangePercent() {}

dtTransformer *pickVec3dTwoDRangePercent::clone(void) const
{
  return new pickVec3dTwoDRangePercent(*this);
}

dtTransformer *pickVec3dTwoDRangePercent::create(void) const
{
  return new pickVec3dTwoDRangePercent();
}

lvH_analyticFunction
pickVec3dTwoDRangePercent::apply(lvH_analyticFunction const *const aFVecP) const
{
  lvH_analyticFunction ret;

  dt__forAllRefAuto(*aFVecP, aFun)
  {
    dt__ptrAss(
      vec3dSurfaceTwoD const *const v3dSurface,
      vec3dSurfaceTwoD::ConstDownCast(aFun)
    );

    dt__pH(dtCurve) dtC;
    if (config().contains("_x0"))
    {
      dtC.reset(v3dSurface->ptrDtSurface()->segmentConstUPercent(
        config().lookup<dtReal>("_x0")
      ));
    }
    else if (config().contains("_x1"))
    {
      dtC.reset(v3dSurface->ptrDtSurface()->segmentConstVPercent(
        config().lookup<dtReal>("_x1")
      ));
    }
    else
      dt__throwUnexpected(apply());

    ret.push_back(new vec3dCurveOneD(dtC.get()));
  }

  return ret;
}

bool pickVec3dTwoDRangePercent::isNecessary(void) const { return true; }

void pickVec3dTwoDRangePercent::jInit(
  jsonPrimitive const &jE,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG
)
{
  dtTransformer::jInit(jE, bC, cV, aF, aG);

  int dirSet = 0;
  if (config().contains("_x0"))
    dirSet = dirSet + 1;
  if (config().contains("_x1"))
    dirSet = dirSet + 1;
  if (dirSet != 1)
  {
    dt__throw(
      jInit(),
      << config().toStdString() << std::endl
      << "Only one value should be greater than zero. Check input."
    );
  }
}

void pickVec3dTwoDRangePercent::init(
  ::QDomElement const *tE,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG
)
{
  dtTransformer::init(tE, bC, cV, aF, aG);

  jsonPrimitive config;

  if (dtXmlParserBase::hasAttribute("x_percent_one", *tE))
  {
    config.append(
      "_x0",
      dtXmlParserBase::muParseString(dtXmlParserBase::replaceDependencies(
        dtXmlParserBase::getAttributeStr("x_percent_one", *tE), cV, aF
      ))
    );
  }
  if (dtXmlParserBase::hasAttribute("x_percent_two", *tE))
  {
    config.append(
      "_x1",
      dtXmlParserBase::muParseString(dtXmlParserBase::replaceDependencies(
        dtXmlParserBase::getAttributeStr("x_percent_two", *tE), cV, aF
      ))
    );
  }

  jInit(config, bC, cV, aF, aG);
}
} // namespace dtOO
