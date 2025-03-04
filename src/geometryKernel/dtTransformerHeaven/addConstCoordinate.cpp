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

#include "addConstCoordinate.h"
#include "baseContainerHeaven/baseContainer.h"
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

#include "dtTransformerFactory.h"
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/vec2dCurve2dOneD.h>
#include <analyticFunctionHeaven/vec3dCurveOneD.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/geoBuilder/geomCurve_convertGeomCurve2d.h>

namespace dtOO {
bool addConstCoordinate::_registrated = dtTransformerFactory::registrate(
  dt__tmpPtr(addConstCoordinate, new addConstCoordinate())
);

addConstCoordinate::addConstCoordinate(jsonPrimitive const &jE)
  : dtTransformer(jE)
{
  this->jInit(jE, NULL, NULL, NULL, NULL);
}

addConstCoordinate::addConstCoordinate() : dtTransformer() {}

addConstCoordinate::addConstCoordinate(const addConstCoordinate &orig)
  : dtTransformer(orig)
{
}

addConstCoordinate::~addConstCoordinate() {}

dtTransformer *addConstCoordinate::clone(void) const
{
  return new addConstCoordinate(*this);
}

dtTransformer *addConstCoordinate::create(void) const
{
  return new addConstCoordinate();
}

lvH_analyticFunction
addConstCoordinate::apply(lvH_analyticFunction const *const aFVecP) const
{
  lvH_analyticFunction aFVecRet;

  dt__forAllIndex(*aFVecP, ii)
  {
    dt__ptrAss(
      vec2dCurve2dOneD const *v2d1d, vec2dCurve2dOneD::DownCast(aFVecP->at(ii))
    );

    dt__pH(dtCurve) dtC(geomCurve_convertGeomCurve2d(
                          v2d1d->ptrDtCurve2d(),
                          CGAL::ORIGIN + config().lookup<dtVector3>("_vv") *
                                           config().lookup<dtReal>("_cc"),
                          config().lookup<dtVector3>("_vv")
    )
                          .result());
    aFVecRet.push_back(new vec3dCurveOneD(dtC.get()));
  }

  return aFVecRet;
}

bool addConstCoordinate::isNecessary(void) const { return true; }

void addConstCoordinate::jInit(
  jsonPrimitive const &jE,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG
)
{
  dtTransformer::jInit(jE, bC, cV, aF, aG);
}

void addConstCoordinate::init(
  ::QDomElement const *tE,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG
)
{
  dtTransformer::init(tE, bC, cV, aF, aG);

  jsonPrimitive config;
  config.append(
    "_cc",
    dtXmlParserBase::muParseString(dtXmlParserBase::replaceDependencies(
      dtXmlParserBase::getAttributeStr("coordinate_value", *tE), cV, aF
    ))
  );
  config.append(
    "_vv",
    dtXmlParserBase::getDtVector3(
      dtXmlParserBase::getChild("Vector_3", *tE), bC
    )
  );
  jInit(config, bC, cV, aF, aG);
}
} // namespace dtOO
