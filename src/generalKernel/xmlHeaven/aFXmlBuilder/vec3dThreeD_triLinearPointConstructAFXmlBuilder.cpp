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

#include "vec3dThreeD_triLinearPointConstructAFXmlBuilder.h"

#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/vec3dTriLinearThreeD.h>
#include <baseContainerHeaven/baseContainer.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/logMe.h>
#include <xmlHeaven/aFXmlBuilderFactory.h>
#include <xmlHeaven/dtXmlParserBase.h>

#include <QtXml/QDomElement>
#include <boost/assign.hpp>

namespace dtOO {
bool vec3dThreeD_triLinearPointConstructAFXmlBuilder::_registrated =
  aFXmlBuilderFactory::registrate(dt__tmpPtr(
    vec3dThreeD_triLinearPointConstructAFXmlBuilder,
    new vec3dThreeD_triLinearPointConstructAFXmlBuilder()
  ));

vec3dThreeD_triLinearPointConstructAFXmlBuilder ::
  vec3dThreeD_triLinearPointConstructAFXmlBuilder()
{
}

vec3dThreeD_triLinearPointConstructAFXmlBuilder ::
  ~vec3dThreeD_triLinearPointConstructAFXmlBuilder()
{
}

std::vector<std::string>
vec3dThreeD_triLinearPointConstructAFXmlBuilder ::factoryAlias(void) const
{
  return ::boost::assign::list_of("vec3dTriLinearThreeD")(
    "vec3dTriLinearThreeDAFXmlBuilder"
  );
}

void vec3dThreeD_triLinearPointConstructAFXmlBuilder::buildPart(
  ::QDomElement const &toBuildP,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticFunction *result
) const
{
  dt__throwIf(!dtXmlParserBase::hasChild("Point_3", toBuildP), buildPart());

  std::vector<dtPoint3> pA;
  std::vector<::QDomElement> wEl =
    dtXmlParserBase::getChildVector("Point_3", toBuildP);

  dt__forAllRefAuto(wEl, anEl)
  {
    std::vector<dtPoint3> wPoint;
    dtXmlParserBase::createBasic(&anEl, bC, cV, aF, &wPoint);
    dt__forAllRefAuto(wPoint, aPoint) pA.push_back(aPoint);
  }

  //
  // create trilinear interpolation
  //
  if (pA.size() == 8)
  {
    result->push_back(new vec3dTriLinearThreeD(
      pA[0], pA[1], pA[2], pA[3], pA[4], pA[5], pA[6], pA[7]
    ));
  }
  else if (pA.size() == 2)
  {
    result->push_back(new vec3dTriLinearThreeD(pA[0], pA[1]));
  }
  else
    dt__throwUnexpected(buildPart());
}
} // namespace dtOO
