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

#include "bSplineSurface_geomCurveFillConstructOCCAGXmlBuilder.h"

#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticCurve.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <dtLinearAlgebra.h>
#include <geometryEngine/dtOCCBezierCurve.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/geoBuilder/bSplineSurface_geomCurveFillConstructOCC.h>
#include <logMe/logMe.h>
#include <xmlHeaven/aGXmlBuilderFactory.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

#include <boost/assign.hpp>

namespace dtOO {
bool bSplineSurface_geomCurveFillConstructOCCAGXmlBuilder::_registrated =
  aGXmlBuilderFactory::registrate(dt__tmpPtr(
    bSplineSurface_geomCurveFillConstructOCCAGXmlBuilder,
    new bSplineSurface_geomCurveFillConstructOCCAGXmlBuilder()
  ));

bSplineSurface_geomCurveFillConstructOCCAGXmlBuilder ::
  bSplineSurface_geomCurveFillConstructOCCAGXmlBuilder()
{
}

bSplineSurface_geomCurveFillConstructOCCAGXmlBuilder ::
  ~bSplineSurface_geomCurveFillConstructOCCAGXmlBuilder()
{
}

std::vector<std::string>
bSplineSurface_geomCurveFillConstructOCCAGXmlBuilder::factoryAlias(void) const
{
  return ::boost::assign::list_of("bSplineSurface_bSplineCurveFillConstructOCC"
  );
}

void bSplineSurface_geomCurveFillConstructOCCAGXmlBuilder::buildPart(
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

  lvH_analyticGeometry aG_t;
  ::QDomElement wElement =
    dtXmlParserBase::getChild("analyticGeometry", toBuild);
  while (!wElement.isNull())
  {
    dtXmlParserBase::createAdvanced(&wElement, bC, cV, aF, aG, &aG_t);
    wElement = dtXmlParserBase::getNextSibling("analyticGeometry", wElement);
  }

  vectorHandling<dtCurve const *> dtC(aG_t.size());
  dt__forAllIndex(aG_t, ii)
  {
    dt__ptrAss(analyticCurve const *s3, analyticCurve::ConstDownCast(aG_t[ii]));
    dtC[ii] = s3->ptrConstDtCurve();
  }

  ptrHandling<dtSurface> dtS(
    bSplineSurface_geomCurveFillConstructOCC(dtC).result()
  );
  result->push_back(new analyticSurface(dtS.get()));
}
} // namespace dtOO
