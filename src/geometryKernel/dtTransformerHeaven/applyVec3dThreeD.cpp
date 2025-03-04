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

#include "applyVec3dThreeD.h"

#include "dtTransformerFactory.h"
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/vec3dThreeD.h>
#include <interfaceHeaven/systemHandling.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>
#include <xmlHeaven/dtXmlParserBase.h>

namespace dtOO {
bool applyVec3dThreeD::_registrated = dtTransformerFactory::registrate(
  dt__tmpPtr(applyVec3dThreeD, new applyVec3dThreeD())
);

applyVec3dThreeD::applyVec3dThreeD() : dtTransformer() {}

applyVec3dThreeD::applyVec3dThreeD(applyVec3dThreeD const &orig)
  : dtTransformer(orig)
{
  _v3.reset(orig._v3->clone());
}

applyVec3dThreeD::applyVec3dThreeD(vec3dThreeD const *const v3)
  : dtTransformer()
{
  _v3.reset(v3->clone());
}

applyVec3dThreeD::~applyVec3dThreeD() {}

dtTransformer *applyVec3dThreeD::clone(void) const
{
  return new applyVec3dThreeD(*this);
}

dtTransformer *applyVec3dThreeD::create(void) const
{
  return new applyVec3dThreeD();
}

std::vector<dtPoint3>
applyVec3dThreeD::apply(std::vector<dtPoint3> const *const toTrans) const
{
  std::vector<dtPoint3> retVec;
  dt__forAllRefAuto(*toTrans, pp)
  {
    retVec.push_back(_v3->YdtPoint3(pp[0], pp[1], pp[2]));
  }

  return retVec;
}

std::vector<dtPoint3>
applyVec3dThreeD::retract(std::vector<dtPoint3> const *const toRetract) const
{
  std::vector<dtPoint3> retVec;
  dt__forAllRefAuto(*toRetract, pp)
  {
    aFX xx = _v3->invY(analyticFunction::aFYThreeD(pp));
    retVec.push_back(dtPoint3(xx[0], xx[1], xx[2]));
  }

  return retVec;
}

bool applyVec3dThreeD::isNecessary(void) const { return true; }

void applyVec3dThreeD::jInit(
  jsonPrimitive const &jE,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG
)
{
  dtTransformer::jInit(jE, bC, cV, aF, aG);

  _v3 =
    vec3dThreeD::PointerDownCast(jE.lookupClone<analyticFunction>("_v3", aF));
}

void applyVec3dThreeD::init(
  ::QDomElement const *tE,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG
)
{
  dt__throwIf(!dtXmlParserBase::hasChild("analyticFunction", *tE), init());

  dtTransformer::init(tE, bC, cV, aF, aG);

  _v3.reset(vec3dThreeD::MustDownCast(dtXmlParserBase::createAnalyticFunction(
    dtXmlParserBase::getChild("analyticFunction", *tE), bC, cV, aF
  )));
}
} // namespace dtOO
