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

#include "map2dTo3dApproximateInMap3dTo3dAGXmlBuilder.h"

#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/aGBuilder/map2dTo3d_approximateMap2dTo3dInMap3dTo3d.h>
#include <analyticGeometryHeaven/aGBuilder/map2dTo3d_fullExtentInMap3dTo3d.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <dtLinearAlgebra.h>
#include <logMe/logMe.h>
#include <xmlHeaven/aGXmlBuilderFactory.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
bool map2dTo3dApproximateInMap3dTo3dAGXmlBuilder::_registrated =
  aGXmlBuilderFactory::registrate(dt__tmpPtr(
    map2dTo3dApproximateInMap3dTo3dAGXmlBuilder,
    new map2dTo3dApproximateInMap3dTo3dAGXmlBuilder()
  ));

map2dTo3dApproximateInMap3dTo3dAGXmlBuilder ::
  map2dTo3dApproximateInMap3dTo3dAGXmlBuilder()
{
}

map2dTo3dApproximateInMap3dTo3dAGXmlBuilder ::
  ~map2dTo3dApproximateInMap3dTo3dAGXmlBuilder()
{
}

void map2dTo3dApproximateInMap3dTo3dAGXmlBuilder::buildPart(
  ::QDomElement const &toBuild,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG,
  lvH_analyticGeometry *result
) const
{
  dt__throwIf(
    !dtXmlParserBase::getNChildren("analyticGeometry", toBuild) ||
      !dtXmlParserBase::hasAttribute("number_points_one", toBuild) ||
      !dtXmlParserBase::hasAttribute("number_points_two", toBuild) ||
      !dtXmlParserBase::hasAttribute("order", toBuild),
    buildPart()
  );

  map2dTo3d const *m2d = NULL;
  map3dTo3d const *m3d = NULL;
  ::QDomElement wElement =
    dtXmlParserBase::getChild("analyticGeometry", toBuild);
  while (!wElement.isNull())
  {
    dt__pH(analyticGeometry)
      aG_t(dtXmlParserBase::createAnalyticGeometry(&wElement, bC, cV, aF, aG));
    m2d = map2dTo3d::SecureCast(aG_t.get());
    m3d = map3dTo3d::SecureCast(aG_t.get());
    wElement = dtXmlParserBase::getNextSibling("analyticGeometry", wElement);
  }
  dt__throwIf(!m2d && !m3d, buildPart());

  int nU = dtXmlParserBase::getAttributeIntMuParse(
    "number_points_one", toBuild, cV, aF
  );
  int nV = dtXmlParserBase::getAttributeIntMuParse(
    "number_points_two", toBuild, cV, aF
  );
  int order = dtXmlParserBase::getAttributeIntMuParse("order", toBuild, cV, aF);

  result->push_back(
    map2dTo3d_approximateMap2dTo3dInMap3dTo3d(m2d, m3d, nU, nV, order).result()
  );
}
} // namespace dtOO
