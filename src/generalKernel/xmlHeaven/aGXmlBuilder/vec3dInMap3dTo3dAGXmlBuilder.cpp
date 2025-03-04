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

#include "vec3dInMap3dTo3dAGXmlBuilder.h"

#include "analyticFunctionHeaven/vec3dTransVolThreeD.h"
#include <xmlHeaven/aGXmlBuilderFactory.h>

#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/vec3dOneD.h>
#include <analyticFunctionHeaven/vec3dThreeD.h>
#include <analyticFunctionHeaven/vec3dTwoD.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <analyticGeometryHeaven/vec3dOneDInMap3dTo3d.h>
#include <analyticGeometryHeaven/vec3dThreeDInMap3dTo3d.h>
#include <analyticGeometryHeaven/vec3dTwoDInMap3dTo3d.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <dtLinearAlgebra.h>
#include <logMe/logMe.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
bool vec3dInMap3dTo3dAGXmlBuilder::_registrated =
  aGXmlBuilderFactory::registrate(
    dt__tmpPtr(vec3dInMap3dTo3dAGXmlBuilder, new vec3dInMap3dTo3dAGXmlBuilder())
  );

vec3dInMap3dTo3dAGXmlBuilder::vec3dInMap3dTo3dAGXmlBuilder() {}

vec3dInMap3dTo3dAGXmlBuilder::~vec3dInMap3dTo3dAGXmlBuilder() {}

void vec3dInMap3dTo3dAGXmlBuilder::buildPart(
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
  lvH_analyticGeometry aGV;
  dtXmlParserBase::createAdvanced(&aGElement, bC, cV, aF, aG, &aGV);

  //
  // get analyticFunction
  //
  ::QDomElement aFElement =
    dtXmlParserBase::getChild("analyticFunction", toBuild);
  dt__pH(analyticFunction const)
    aF_t(dtXmlParserBase::createAnalyticFunction(&aFElement, bC, cV, aF));

  vec3dOneD const *v1d = vec3dOneD::ConstDownCast(aF_t.get());
  vec3dTwoD const *v2d = vec3dTwoD::ConstDownCast(aF_t.get());
  vec3dThreeD const *v3d = vec3dThreeD::ConstDownCast(aF_t.get());

  dt__forAllRefAuto(aGV, anAG)
  {
    //
    // check if it is a map3dTo3d
    //
    map3dTo3d const *const m3d = map3dTo3d::MustConstDownCast(anAG);

    if (v1d)
    {
      result->push_back(new vec3dOneDInMap3dTo3d(v1d, m3d, optionPercent));
    }
    else if (v2d)
    {
      result->push_back(new vec3dTwoDInMap3dTo3d(v2d, m3d, optionPercent));
    }
    else if (v3d)
    {
      result->push_back(new vec3dThreeDInMap3dTo3d(v3d, m3d, optionPercent));
    }
    else
      dt__throwUnexpected(buildPart());
  }
  aGV.destroy();
}

void vec3dInMap3dTo3dAGXmlBuilder::buildPartCompound(
  ::QDomElement const &toBuild,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG,
  lvH_analyticGeometry *result
) const
{
  dt__throwIf(
    !dtXmlParserBase::hasChild("analyticGeometry", toBuild), buildPart()
  );
  dt__throwIf(
    !dtXmlParserBase::hasChild("analyticFunction", toBuild), buildPart()
  );

  //    if ( hasAG && hasAF ) {
  //
  // get analyticGeometry
  //
  analyticGeometry const *aG_t = aG->get(dtXmlParserBase::getAttributeStr(
    "label", dtXmlParserBase::getChild("analyticGeometry", toBuild)
  ));
  //
  // check if it is a map3dTo3d
  //
  dt__ptrAss(map3dTo3d const *m3d, map3dTo3d::ConstDownCast(aG_t));

  //
  // get analyticFunction
  //
  analyticFunction const *aF_t = aF->get(dtXmlParserBase::getAttributeStr(
    "label", dtXmlParserBase::getChild("analyticFunction", toBuild)
  ));

  //			vec3dOneD const * v1d = vec3dOneD::ConstDownCast(aF);
  //			vec3dTwoD const * v2d = vec3dTwoD::ConstDownCast(aF);
  vec3dTransVolThreeDCompound const *v3d =
    vec3dTransVolThreeDCompound::ConstDownCast(aF_t);
  //
  //			if (v1d) {
  //				aGeoP->push_back( new vec3dOneDInMap3dTo3d(v1d,
  //m3d) );
  //			}
  //			else if (v2d) {
  //			  aGeoP->push_back( new vec3dTwoDInMap3dTo3d(v2d, m3d)
  //);
  //			}
  if (v3d)
  {
    vec3dThreeDInMap3dTo3dCompound *v3dC = new vec3dThreeDInMap3dTo3dCompound();
    for (int ii = 0; ii < v3d->nComponents(); ii++)
    {
      v3dC->addComponent(vec3dThreeDInMap3dTo3d(&(v3d->component(ii)), m3d));
      //			    aGeoP->push_back( new
      //vec3dThreeDInMap3dTo3d(v3d, m3d) );
    }
    result->push_back(v3dC);
  }
  else
  {
    dt__throw(
      buildPart(),
      //					<< dt__eval(v1d) << std::endl
      //					<< dt__eval(v2d) << std::endl
      << dt__eval(v3d)
    );
  }
  //    }
}
} // namespace dtOO
