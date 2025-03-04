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

#include "bSplineCurve_pointConstructOCCAFXmlBuilder.h"

#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/vec3dCurveOneD.h>
#include <baseContainerHeaven/baseContainer.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/geoBuilder/bSplineCurve_pointConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve_pointInterpolateConstructOCC.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/logMe.h>
#include <xmlHeaven/aFXmlBuilderFactory.h>
#include <xmlHeaven/dtXmlParser.h>
#include <xmlHeaven/dtXmlParserBase.h>

#include <QtXml/QDomElement>
#include <boost/assign.hpp>

namespace dtOO {
bool bSplineCurve_pointConstructOCCAFXmlBuilder::_registrated =
  aFXmlBuilderFactory::registrate(dt__tmpPtr(
    bSplineCurve_pointConstructOCCAFXmlBuilder,
    new bSplineCurve_pointConstructOCCAFXmlBuilder()
  ));

bSplineCurve_pointConstructOCCAFXmlBuilder ::
  bSplineCurve_pointConstructOCCAFXmlBuilder()
{
}

bSplineCurve_pointConstructOCCAFXmlBuilder ::
  ~bSplineCurve_pointConstructOCCAFXmlBuilder()
{
}

std::vector<std::string>
bSplineCurve_pointConstructOCCAFXmlBuilder ::factoryAlias(void) const
{
  return ::boost::assign::list_of("vec3dCurveOneD")("vec3dCurveOneDAFXmlBuilder"
  );
}

void bSplineCurve_pointConstructOCCAFXmlBuilder::buildPart(
  ::QDomElement const &toBuildP,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticFunction *result
) const
{
  dt__throwIf(!dtXmlParserBase::hasChild("Point_3", toBuildP), buildPart());
  //
  // get points
  //
  std::vector<dtPoint3> points;
  dt__forAllRefAuto(dtXmlParserBase::getChildVector("Point_3", toBuildP), anEl)
  {
    std::vector<dtPoint3> wP;
    dtXmlParserBase::dtXmlParserBase::createBasic(&anEl, bC, cV, aF, &wP);
    dt__forAllRefAuto(wP, aWP) points.push_back(aWP);
  }

  //
  // construct by control points
  //
  ptrHandling<dtCurve> dtC;
  if ((dtXmlParser::getAttributeStr("via", toBuildP, "") == "") ||
      (dtXmlParser::getAttributeStr("via", toBuildP, "") == "pointConstruct"))
  {
    dt__throwIf(!dtXmlParserBase::hasAttribute("order", toBuildP), buildPart());
    dtC.reset(bSplineCurve_pointConstructOCC(
                points, dtXmlParserBase::getAttributeInt("order", toBuildP)
    )
                .result());
  }
  //
  // construct by pointInterpolate
  //
  else if (dtXmlParser::getAttributeStr("via", toBuildP, "") ==
           "pointInterpolateConstruct")
  {
    dtC.reset(bSplineCurve_pointInterpolateConstructOCC(
                points,
                dtXmlParserBase::getAttributeIntMuParse(
                  "orderMin", toBuildP, cV, aF, 3
                ),
                dtXmlParserBase::getAttributeIntMuParse(
                  "orderMax", toBuildP, cV, aF, 8
                ),
                dtXmlParserBase::getAttributeIntMuParse(
                  "continuity", toBuildP, cV, aF, 4
                )
    )
                .result());
  }
  else
    dt__throwUnexpected(buildPart());

  //
  // create vec3dCurveOneD
  //
  result->push_back(new vec3dCurveOneD(dtC.get()));
}
} // namespace dtOO
