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

#include "radialTranslatingMap2dTo3dAGXmlBuilder.h"

#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/radialTranslatingMap2dTo3d.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <dtLinearAlgebra.h>
#include <logMe/logMe.h>
#include <xmlHeaven/aGXmlBuilderFactory.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
bool radialTranslatingMap2dTo3dAGXmlBuilder::_registrated =
  aGXmlBuilderFactory::registrate(dt__tmpPtr(
    radialTranslatingMap2dTo3dAGXmlBuilder,
    new radialTranslatingMap2dTo3dAGXmlBuilder()
  ));

radialTranslatingMap2dTo3dAGXmlBuilder ::radialTranslatingMap2dTo3dAGXmlBuilder(
)
{
}

radialTranslatingMap2dTo3dAGXmlBuilder ::
  ~radialTranslatingMap2dTo3dAGXmlBuilder()
{
}

void radialTranslatingMap2dTo3dAGXmlBuilder::buildPart(
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
    !dtXmlParserBase::hasChild("Vector_3", toBuild) &&
      !dtXmlParserBase::hasAttribute("distance", toBuild),
    buildPart()
  );

  //
  // get analyticGeometry and cast
  //
  dt__pH(analyticGeometry) aG_t(dtXmlParserBase::createAnalyticGeometry(
    dtXmlParserBase::getChild("analyticGeometry", toBuild), bC, cV, aF, aG
  ));
  map2dTo3d const *m2d = map2dTo3d::MustConstDownCast(aG_t.get());

  //
  // get vector
  //
  ::QDomElement wElement = dtXmlParserBase::getChild("Vector_3", toBuild);
  dtVector3 vv = dtXmlParserBase::createDtVector3(&wElement, bC, cV, aF, aG);
  dtReal distance =
    dtXmlParserBase::getAttributeFloatMuParse("distance", toBuild, cV, aF, aG);
  //
  // create analyticGeometry
  //
  result->push_back(new radialTranslatingMap2dTo3d(vv, distance, m2d));
}
} // namespace dtOO
