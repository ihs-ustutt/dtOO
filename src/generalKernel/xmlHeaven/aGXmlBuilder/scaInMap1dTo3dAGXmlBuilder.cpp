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

#include "scaInMap1dTo3dAGXmlBuilder.h"

#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/scaOneD.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/scaOneDInMap1dTo3d.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <dtLinearAlgebra.h>
#include <logMe/logMe.h>
#include <xmlHeaven/aGXmlBuilderFactory.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
bool scaInMap1dTo3dAGXmlBuilder::_registrated = aGXmlBuilderFactory::registrate(
  dt__tmpPtr(scaInMap1dTo3dAGXmlBuilder, new scaInMap1dTo3dAGXmlBuilder())
);

scaInMap1dTo3dAGXmlBuilder::scaInMap1dTo3dAGXmlBuilder() {}

scaInMap1dTo3dAGXmlBuilder::~scaInMap1dTo3dAGXmlBuilder() {}

void scaInMap1dTo3dAGXmlBuilder::buildPart(
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
  dt__throwIf(
    !dtXmlParserBase::hasChild("analyticFunction", toBuild), buildPart()
  );
  bool optionPercent = dtXmlParserBase::getAttributeBool("percent", toBuild);

  //
  // get analyticGeometry
  //
  ::QDomElement aGElement =
    dtXmlParserBase::getChild("analyticGeometry", toBuild);
  dt__pH(analyticGeometry const)
    aG_t(dtXmlParserBase::createAnalyticGeometry(&aGElement, bC, cV, aF, aG));

  //
  // check if it is a map1dTo3d
  //
  dt__ptrAss(map1dTo3d const *m1d, map1dTo3d::ConstDownCast(aG_t.get()));

  //
  // get analyticFunction
  //
  ::QDomElement aFElement =
    dtXmlParserBase::getChild("analyticFunction", toBuild);
  dt__pH(analyticFunction const)
    aF_t(dtXmlParserBase::createAnalyticFunction(&aFElement, bC, cV, aF));
  dt__ptrAss(scaOneD const *s1d, scaOneD::ConstDownCast(aF_t.get()));

  //
  // create and return scaOneDInMap1dTo3d
  //
  result->push_back(new scaOneDInMap1dTo3d(s1d, m1d, optionPercent));
}
} // namespace dtOO
