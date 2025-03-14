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

#include "scaOneD_forceBSplineCurve2dAFXmlBuilder.h"
#include "xmlHeaven/dtXmlParser.h"

#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/scaCurve2dOneD.h>
#include <baseContainerHeaven/baseContainer.h>
#include <geometryEngine/dtCurve2d.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_pointConstructOCC.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/logMe.h>
#include <xmlHeaven/aFXmlBuilderFactory.h>
#include <xmlHeaven/dtXmlParserBase.h>

#include <QtXml/QDomElement>
#include <boost/assign.hpp>

namespace dtOO {
bool scaOneD_forceBSplineCurve2dAFXmlBuilder::_registrated =
  aFXmlBuilderFactory::registrate(dt__tmpPtr(
    scaOneD_forceBSplineCurve2dAFXmlBuilder,
    new scaOneD_forceBSplineCurve2dAFXmlBuilder()
  ));

scaOneD_forceBSplineCurve2dAFXmlBuilder ::
  scaOneD_forceBSplineCurve2dAFXmlBuilder()
{
}

scaOneD_forceBSplineCurve2dAFXmlBuilder ::
  ~scaOneD_forceBSplineCurve2dAFXmlBuilder()
{
}

std::vector<std::string>
scaOneD_forceBSplineCurve2dAFXmlBuilder ::factoryAlias(void) const
{
  return ::boost::assign::list_of("scaCurve2dOneD");
}

void scaOneD_forceBSplineCurve2dAFXmlBuilder::buildPart(
  ::QDomElement const &toBuildP,
  baseContainer *const bC,
  lvH_constValue const *const cValP,
  lvH_analyticFunction const *const depSFunP,
  lvH_analyticFunction *sFunP
) const
{
  dt__throwIf(
    !dtXmlParserBase::hasAttribute("order", toBuildP) ||
      !dtXmlParserBase::hasChild("Point_2", toBuildP),
    buildPart()
  );

  std::vector<dtPoint2> pointsArray;
  ::QDomElement elementP = dtXmlParserBase::getChild("Point_2", toBuildP);
  //
  // set input
  //
  dtInt order =
    dtXmlParserBase::getAttributeIntMuParse("order", toBuildP, cValP);
  while (!elementP.isNull())
  {
    std::vector<dtPoint2> workingPoint;
    dtXmlParserBase::dtXmlParserBase::createBasic(
      &elementP, bC, cValP, depSFunP, &workingPoint
    );
    for (int ii = 0; ii < workingPoint.size(); ii++)
    {
      pointsArray.push_back(workingPoint[ii]);
      //					delete workingPoint[ii];
    }
    // next sibling
    elementP =
      ::QDomElement(dtXmlParserBase::getNextSibling("Point_2", elementP));
  }
  ptrHandling<dtCurve2d> dtC2d(
    bSplineCurve2d_pointConstructOCC(pointsArray, order).result()
  );

  //
  // create scaCurve2dOneD
  //
  sFunP->push_back(new scaCurve2dOneD(dtC2d.get()));
}
} // namespace dtOO
