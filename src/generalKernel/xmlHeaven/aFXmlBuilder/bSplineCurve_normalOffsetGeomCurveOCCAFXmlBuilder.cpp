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

#include "bSplineCurve_normalOffsetGeomCurveOCCAFXmlBuilder.h"

#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/vec3dCurveOneD.h>
#include <baseContainerHeaven/baseContainer.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/geoBuilder/bSplineCurve_normalOffsetGeomCurveOCC.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/logMe.h>
#include <xmlHeaven/aFXmlBuilderFactory.h>
#include <xmlHeaven/dtXmlParser.h>
#include <xmlHeaven/dtXmlParserBase.h>

#include <QtXml/QDomElement>

namespace dtOO {
bool bSplineCurve_normalOffsetGeomCurveOCCAFXmlBuilder::_registrated =
  aFXmlBuilderFactory::registrate(dt__tmpPtr(
    bSplineCurve_normalOffsetGeomCurveOCCAFXmlBuilder,
    new bSplineCurve_normalOffsetGeomCurveOCCAFXmlBuilder()
  ));

bSplineCurve_normalOffsetGeomCurveOCCAFXmlBuilder ::
  bSplineCurve_normalOffsetGeomCurveOCCAFXmlBuilder()
{
}

bSplineCurve_normalOffsetGeomCurveOCCAFXmlBuilder ::
  ~bSplineCurve_normalOffsetGeomCurveOCCAFXmlBuilder()
{
}

void bSplineCurve_normalOffsetGeomCurveOCCAFXmlBuilder::buildPart(
  ::QDomElement const &toBuildP,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticFunction *result
) const
{
  dt__throwIf(
    !dtXmlParserBase::hasChild("Vector_3", toBuildP) ||
      !dtXmlParserBase::hasChild("analyticFunction", toBuildP) ||
      !dtXmlParserBase::hasAttribute("distance", toBuildP),
    buildPart()
  );

  //
  // get distance
  //
  dtReal distance =
    dtXmlParserBase::getAttributeFloatMuParse("distance", toBuildP, cV, aF);

  //
  // get base vector
  //
  QDomElement vEl = dtXmlParserBase::getChild("Vector_3", toBuildP);
  dtVector3 base =
    dtXmlParserBase::dtXmlParserBase::getDtVector3(&vEl, bC, cV, aF);

  //
  // get analytic function
  //
  ::QDomElement aFEl = dtXmlParserBase::getChild("analyticFunction", toBuildP);
  dt__ptrAss(
    vec3dCurveOneD * s3,
    vec3dCurveOneD::DownCast(
      dtXmlParserBase::createAnalyticFunction(&aFEl, bC, cV, aF)
    )
  );

  //
  // offset curve
  //
  ptrHandling<dtCurve> dtC(
    bSplineCurve_normalOffsetGeomCurveOCC(s3->ptrConstDtCurve(), distance, base)
      .result()
  );

  //
  // push back to result
  //
  result->push_back(new vec3dCurveOneD(dtC.get()));
}
} // namespace dtOO
