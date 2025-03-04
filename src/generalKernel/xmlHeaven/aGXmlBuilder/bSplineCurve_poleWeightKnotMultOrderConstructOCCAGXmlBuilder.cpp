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

#include "bSplineCurve_poleWeightKnotMultOrderConstructOCCAGXmlBuilder.h"

#include "analyticGeometryHeaven/analyticCurve.h"
#include <xmlHeaven/aGXmlBuilderFactory.h>

#include <analyticGeometryHeaven/analyticCurve.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <baseContainerHeaven/baseContainer.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/geoBuilder/bSplineCurve_poleWeightKnotMultOrderConstructOCC.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/logMe.h>
#include <xmlHeaven/dtXmlParser.h>
#include <xmlHeaven/dtXmlParserBase.h>

#include <QtXml/QDomElement>

namespace dtOO {
bool
  bSplineCurve_poleWeightKnotMultOrderConstructOCCAGXmlBuilder::_registrated =
    aGXmlBuilderFactory::registrate(dt__tmpPtr(
      bSplineCurve_poleWeightKnotMultOrderConstructOCCAGXmlBuilder,
      new bSplineCurve_poleWeightKnotMultOrderConstructOCCAGXmlBuilder()
    ));

bSplineCurve_poleWeightKnotMultOrderConstructOCCAGXmlBuilder ::
  bSplineCurve_poleWeightKnotMultOrderConstructOCCAGXmlBuilder()
{
}

bSplineCurve_poleWeightKnotMultOrderConstructOCCAGXmlBuilder ::
  ~bSplineCurve_poleWeightKnotMultOrderConstructOCCAGXmlBuilder()
{
}

void bSplineCurve_poleWeightKnotMultOrderConstructOCCAGXmlBuilder::buildPart(
  ::QDomElement const &toBuild,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG,
  lvH_analyticGeometry *result
) const
{
  dt__throwIf(
    !dtXmlParserBase::hasChild("Point_3", toBuild) ||
      !dtXmlParserBase::hasAttribute("weights|knots|mults|order", toBuild),
    buildPart()
  );
  //
  // get points
  //
  std::vector<dtPoint3> poles;
  dt__forAllRefAuto(dtXmlParserBase::getChildVector("Point_3", toBuild), anEl)
  {
    std::vector<dtPoint3> wP;
    dtXmlParserBase::dtXmlParserBase::createBasic(&anEl, bC, cV, aF, aG, &wP);
    dt__forAllRefAuto(wP, aWP) poles.push_back(aWP);
  }

  //
  // parse weights, knots, mults, order and call geoBuilder
  //
  dt__pH(dtCurve) dtC(
    bSplineCurve_poleWeightKnotMultOrderConstructOCC(
      poles,
      dtXmlParserBase::getAttributeFloatVectorMuParse(
        "weights", toBuild, cV, aF
      ),
      dtXmlParserBase::getAttributeFloatVectorMuParse("knots", toBuild, cV, aF),
      dtXmlParserBase::getAttributeIntVectorMuParse("mults", toBuild, cV, aF),
      dtXmlParserBase::getAttributeIntMuParse("order", toBuild, cV),
      dtXmlParserBase::getAttributeBool("periodic", toBuild, false)
    )
      .result()
  );

  //
  // create vec3dCurveOneD
  //
  result->push_back(new analyticCurve(dtC.get()));
}
} // namespace dtOO
