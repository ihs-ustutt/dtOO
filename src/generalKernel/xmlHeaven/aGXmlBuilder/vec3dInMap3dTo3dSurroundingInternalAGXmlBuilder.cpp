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

#include "vec3dInMap3dTo3dSurroundingInternalAGXmlBuilder.h"

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
bool vec3dInMap3dTo3dSurroundingInternalAGXmlBuilder::_registrated =
  aGXmlBuilderFactory::registrate(dt__tmpPtr(
    vec3dInMap3dTo3dSurroundingInternalAGXmlBuilder,
    new vec3dInMap3dTo3dSurroundingInternalAGXmlBuilder()
  ));

vec3dInMap3dTo3dSurroundingInternalAGXmlBuilder ::
  vec3dInMap3dTo3dSurroundingInternalAGXmlBuilder()
{
}

vec3dInMap3dTo3dSurroundingInternalAGXmlBuilder ::
  ~vec3dInMap3dTo3dSurroundingInternalAGXmlBuilder()
{
}

void vec3dInMap3dTo3dSurroundingInternalAGXmlBuilder::buildPart(
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
    !dtXmlParserBase::getNChildren("analyticGeometry", toBuild), buildPart()
  );
  dt__throwIf(
    !dtXmlParserBase::hasAttribute("number_points_one", toBuild), buildPart()
  );
  dt__throwIf(
    !dtXmlParserBase::hasAttribute("number_points_two", toBuild), buildPart()
  );
  dt__throwIf(!dtXmlParserBase::hasAttribute("order", toBuild), buildPart());

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

  dt__pH(map2dTo3d) toRet(
    map2dTo3d_approximateMap2dTo3dInMap3dTo3d(m2d, m3d, nU, nV, order).result()
  );
  toRet.reset(map2dTo3d_fullExtentInMap3dTo3d(toRet.get(), 0, 1).result());

  result->push_back(toRet.get()->clone());
}
} // namespace dtOO
