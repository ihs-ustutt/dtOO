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

#include "orientTrans6SidedCubeAGXmlBuilder.h"

#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/trans6SidedCube.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <dtLinearAlgebra.h>
#include <geometryEngine/dtSurface.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>
#include <xmlHeaven/aGXmlBuilderFactory.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
bool orientTrans6SidedCubeAGXmlBuilder::_registrated =
  aGXmlBuilderFactory::registrate(dt__tmpPtr(
    orientTrans6SidedCubeAGXmlBuilder, new orientTrans6SidedCubeAGXmlBuilder()
  ));

orientTrans6SidedCubeAGXmlBuilder::orientTrans6SidedCubeAGXmlBuilder() {}

orientTrans6SidedCubeAGXmlBuilder::~orientTrans6SidedCubeAGXmlBuilder() {}

void orientTrans6SidedCubeAGXmlBuilder::buildPart(
  ::QDomElement const &toBuild,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG,
  lvH_analyticGeometry *result
) const
{
  //
  // check input
  //
  dt__throwIf(
    !dtXmlParserBase::hasChild("analyticGeometry", toBuild), buildPart()
  );

  dt__forAllRefAuto(
    dtXmlParserBase::getChildVector("analyticGeometry", toBuild), anE
  )
  {
    //
    // create and cast analyticGeometry
    //
    trans6SidedCube *t6 = trans6SidedCube::MustDownCast(
      dtXmlParserBase::createAnalyticGeometry(anE, bC, cV, aF, aG)
    );

    //
    // modify
    //
    dt__forAllRefAuto(
      dtXmlParserBase::getAttributeStrVector("orient", anE), anOrient
    )
    {
      if (anOrient == "exchangeVW")
        t6->exchangeVW();
      else if (anOrient == "exchangeUV")
        t6->exchangeUV();
      else if (anOrient == "exchangeUW")
        t6->exchangeUW();
      else if (anOrient == "reverseU")
        t6->reverseU();
      else if (anOrient == "reverseV")
        t6->reverseV();
      else if (anOrient == "reverseW")
        t6->reverseW();
      else
        dt__throwUnexpected(buildPart());
    }

    //
    // add to result
    //
    result->push_back(t6);
  }
}
} // namespace dtOO
