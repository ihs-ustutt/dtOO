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

#include "bSplineCurve2d_pointConstructOCCAFXmlBuilder.h"

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/vec2dCurve2dOneD.h>
#include <baseContainerHeaven/baseContainer.h>
#include <dtTransformerHeaven/dtTransformerFactory.h>
#include <geometryEngine/dtCurve2d.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_pointConstructOCC.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/logMe.h>
#include <xmlHeaven/aFXmlBuilderFactory.h>
#include <xmlHeaven/dtXmlParserBase.h>

namespace dtOO {
bool bSplineCurve2d_pointConstructOCCAFXmlBuilder::_registrated =
  aFXmlBuilderFactory::registrate(dt__tmpPtr(
    bSplineCurve2d_pointConstructOCCAFXmlBuilder,
    new bSplineCurve2d_pointConstructOCCAFXmlBuilder()
  ));

bSplineCurve2d_pointConstructOCCAFXmlBuilder ::
  bSplineCurve2d_pointConstructOCCAFXmlBuilder()
{
}

bSplineCurve2d_pointConstructOCCAFXmlBuilder ::
  ~bSplineCurve2d_pointConstructOCCAFXmlBuilder()
{
}

void bSplineCurve2d_pointConstructOCCAFXmlBuilder::buildPart(
  ::QDomElement const &toBuild,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticFunction *result
) const
{
  dt__throwIf(
    !dtXmlParserBase::hasChild("Point_2", toBuild) ||
      !dtXmlParserBase::hasAttribute("order", toBuild),
    buildPart()
  );

  std::vector<dtPoint2> pointsArray;
  ::QDomElement elementP = dtXmlParserBase::getChild("Point_2", toBuild);
  //
  // set input
  //
  dtInt order = dtXmlParserBase::getAttributeInt("order", toBuild);
  while (!elementP.isNull())
  {
    std::vector<dtPoint2> workingPoint;
    dtXmlParserBase::dtXmlParserBase::createBasic(
      &elementP, bC, cV, aF, &workingPoint
    );
    for (int ii = 0; ii < workingPoint.size(); ii++)
    {
      // put in point builder
      pointsArray.push_back(workingPoint[ii]);
    }
    // next sibling
    elementP = dtXmlParserBase::getNextSibling("Point_2", elementP);
  }
  ptrHandling<dtCurve2d> dtC2d(
    bSplineCurve2d_pointConstructOCC(pointsArray, order).result()
  );

  //
  // create scaCurve2dOneD
  //
  result->push_back(new vec2dCurve2dOneD(dtC2d.get()));
}
} // namespace dtOO
