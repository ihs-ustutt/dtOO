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

#include "geomFillBoundWithSurf_surfaceConstructOCCAFXmlBuilder.h"

#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/vec2dCurve2dOneD.h>
#include <analyticFunctionHeaven/vec3dCurveOneD.h>
#include <analyticFunctionHeaven/vec3dSurfaceTwoD.h>
#include <baseContainerHeaven/baseContainer.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/geoBuilder/geomFillBoundWithSurf_surfaceConstructOCC.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/logMe.h>
#include <xmlHeaven/aFXmlBuilderFactory.h>
#include <xmlHeaven/dtXmlParser.h>
#include <xmlHeaven/dtXmlParserBase.h>

#include <QtXml/QDomElement>

namespace dtOO {
bool geomFillBoundWithSurf_surfaceConstructOCCAFXmlBuilder::_registrated =
  aFXmlBuilderFactory::registrate(dt__tmpPtr(
    geomFillBoundWithSurf_surfaceConstructOCCAFXmlBuilder,
    new geomFillBoundWithSurf_surfaceConstructOCCAFXmlBuilder()
  ));

geomFillBoundWithSurf_surfaceConstructOCCAFXmlBuilder ::
  geomFillBoundWithSurf_surfaceConstructOCCAFXmlBuilder()
{
}

geomFillBoundWithSurf_surfaceConstructOCCAFXmlBuilder ::
  ~geomFillBoundWithSurf_surfaceConstructOCCAFXmlBuilder()
{
}

void geomFillBoundWithSurf_surfaceConstructOCCAFXmlBuilder::buildPart(
  ::QDomElement const &toBuildP,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticFunction *result
) const
{
  dt__throwIf(
    !dtXmlParserBase::hasChild("analyticFunction", toBuildP), buildPart()
  );

  //
  // get analytic function
  //
  std::vector<::QDomElement> elVec =
    dtXmlParserBase::getChildVector("analyticFunction", toBuildP);

  dt__ptrAss(
    vec3dSurfaceTwoD * dtS,
    vec3dSurfaceTwoD::DownCast(
      dtXmlParserBase::createAnalyticFunction(elVec[0], bC, cV, aF)
    )
  );
  dt__ptrAss(
    vec2dCurve2dOneD * dtC2d,
    vec2dCurve2dOneD::DownCast(
      dtXmlParserBase::createAnalyticFunction(elVec[1], bC, cV, aF)
    )
  );

  //
  // create boundary curve
  //
  ptrHandling<dtCurve> dtC(geomFillBoundWithSurf_surfaceConstructOCC(
                             dtS->ptrDtSurface(), dtC2d->ptrDtCurve2d()
  )
                             .result());

  //
  // push back to result
  //
  result->push_back(new vec3dCurveOneD(dtC.get()));
}
} // namespace dtOO
