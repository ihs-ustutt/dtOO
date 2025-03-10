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

#include "bSplineCurve_pointPickFromPartConstructOCCAGXmlBuilder.h"
#include <xmlHeaven/aGXmlBuilderFactory.h>

#include "analyticGeometryHeaven/map2dTo3d.h"
#include "geometryEngine/geoBuilder/bSplineCurve_pointConstructOCC.h"

#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticCurve.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <dtLinearAlgebra.h>
#include <geometryEngine/dtOCCCurve.h>
#include <geometryEngine/geoBuilder/bSplineCurve_pointConstructOCC.h>
#include <logMe/logMe.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
bool bSplineCurve_pointPickFromPartConstructOCCAGXmlBuilder::_registrated =
  aGXmlBuilderFactory::registrate(dt__tmpPtr(
    bSplineCurve_pointPickFromPartConstructOCCAGXmlBuilder,
    new bSplineCurve_pointPickFromPartConstructOCCAGXmlBuilder()
  ));

bSplineCurve_pointPickFromPartConstructOCCAGXmlBuilder ::
  bSplineCurve_pointPickFromPartConstructOCCAGXmlBuilder()
{
}

bSplineCurve_pointPickFromPartConstructOCCAGXmlBuilder ::
  ~bSplineCurve_pointPickFromPartConstructOCCAGXmlBuilder()
{
}

void bSplineCurve_pointPickFromPartConstructOCCAGXmlBuilder::buildPart(
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
  dt__throwIf(!dtXmlParserBase::hasChild("Point_2", toBuild), buildPart());
  dt__throwIf(
    !dtXmlParserBase::hasAttribute("part_label", toBuild), buildPart()
  );
  dt__throwIf(!dtXmlParserBase::hasAttribute("order", toBuild), buildPart());

  std::vector<dtPoint2> p2;
  ::QDomElement wElement = dtXmlParserBase::getChild("Point_2", toBuild);
  while (!wElement.isNull())
  {
    dtXmlParserBase::dtXmlParserBase::createBasic(
      &wElement, bC, cV, aF, aG, &p2
    );
    wElement = dtXmlParserBase::getNextSibling("Point_2", wElement);
  }

  //
  // get mapping
  //
  dt__ptrAss(
    map2dTo3d const *const map,
    map2dTo3d::ConstDownCast(
      aG->get(dtXmlParserBase::getAttributeStr("part_label", toBuild))
    )
  );

  std::vector<dtPoint3> p3(p2.size());
  dt__forAllIndex(p2, ii) p3[ii] = map->getPoint(p2[ii]);

  dtInt order = dtXmlParserBase::getAttributeInt("order", toBuild);
  result->push_back(
    new analyticCurve(bSplineCurve_pointConstructOCC(p3, order).result())
  );
}
} // namespace dtOO
