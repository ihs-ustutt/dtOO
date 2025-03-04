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

#include "bSplineSurface_pipeConstructOCCAFXmlBuilder.h"

#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/vec3dCurveOneD.h>
#include <analyticFunctionHeaven/vec3dSurfaceTwoD.h>
#include <baseContainerHeaven/baseContainer.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/geoBuilder/bSplineSurface_exchangeSurfaceConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineSurface_pipeConstructOCC.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/logMe.h>
#include <xmlHeaven/aFXmlBuilderFactory.h>
#include <xmlHeaven/dtXmlParserBase.h>

#include <QtXml/QDomElement>
#include <boost/assign.hpp>

namespace dtOO {
bool bSplineSurface_pipeConstructOCCAFXmlBuilder::_registrated =
  aFXmlBuilderFactory::registrate(dt__tmpPtr(
    bSplineSurface_pipeConstructOCCAFXmlBuilder,
    new bSplineSurface_pipeConstructOCCAFXmlBuilder()
  ));

bSplineSurface_pipeConstructOCCAFXmlBuilder ::
  bSplineSurface_pipeConstructOCCAFXmlBuilder()
{
}

bSplineSurface_pipeConstructOCCAFXmlBuilder ::
  ~bSplineSurface_pipeConstructOCCAFXmlBuilder()
{
}

//  std::vector< std::string > bSplineSurface_pipeConstructOCCAFXmlBuilder
//    ::factoryAlias( void ) const {
//    return ::boost::assign::list_of
//      ("vec3dSurfaceTwoD")
//      ("vec3dSurfaceTwoDAFXmlBuilder");
//  }

void bSplineSurface_pipeConstructOCCAFXmlBuilder::buildPart(
  ::QDomElement const &toBuild,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticFunction *result
) const
{
  dt__throwIf(
    !dtXmlParserBase::hasChild("analyticFunction", toBuild), buildPart()
  );

  bool polynomial =
    dtXmlParserBase::getAttributeBool("polynomial", toBuild, false);
  dtInt absShape =
    dtXmlParserBase::getAttributeIntMuParse("absShape", toBuild, cV, aF, 2);
  dtInt maxDegree =
    dtXmlParserBase::getAttributeIntMuParse("maxDegree", toBuild, cV, aF, 11);
  dtInt nbSections =
    dtXmlParserBase::getAttributeIntMuParse("nbSections", toBuild, cV, aF, 30);

  std::vector<::QDomElement> wE_v =
    dtXmlParserBase::getChildVector("analyticFunction", toBuild);

  dt__pH(dtSurface) dtS;
  if ((wE_v.size() == 1) && dtXmlParserBase::hasAttribute("radius", toBuild))
  {
    dt__pH(analyticFunction)
      aPath(dtXmlParserBase::createAnalyticFunction(wE_v[0], bC, cV, aF));
    dtS.reset(
      bSplineSurface_pipeConstructOCC(
        vec3dCurveOneD::MustDownCast(aPath.get())->ptrConstDtCurve(),
        dtXmlParserBase::getAttributeFloatMuParse("radius", toBuild, cV, aF),
        polynomial,
        absShape,
        maxDegree,
        nbSections
      )
        .result()
    );
  }
  else if (wE_v.size() == 2)
  {
    dt__pH(analyticFunction)
      aPath(dtXmlParserBase::createAnalyticFunction(wE_v[0], bC, cV, aF));
    dt__pH(analyticFunction)
      aSecOne(dtXmlParserBase::createAnalyticFunction(wE_v[1], bC, cV, aF));
    dtS.reset(bSplineSurface_pipeConstructOCC(
                vec3dCurveOneD::MustDownCast(aPath.get())->ptrConstDtCurve(),
                vec3dCurveOneD::MustDownCast(aSecOne.get())->ptrConstDtCurve(),
                polynomial,
                absShape,
                maxDegree,
                nbSections
    )
                .result());
  }
  else if (wE_v.size() == 3)
  {
    dt__pH(analyticFunction)
      aPath(dtXmlParserBase::createAnalyticFunction(wE_v[0], bC, cV, aF));
    dt__pH(analyticFunction)
      aSecOne(dtXmlParserBase::createAnalyticFunction(wE_v[1], bC, cV, aF));
    dt__pH(analyticFunction)
      aSecTwo(dtXmlParserBase::createAnalyticFunction(wE_v[2], bC, cV, aF));
    dtS.reset(bSplineSurface_pipeConstructOCC(
                vec3dCurveOneD::MustDownCast(aPath.get())->ptrConstDtCurve(),
                vec3dCurveOneD::MustDownCast(aSecOne.get())->ptrConstDtCurve(),
                vec3dCurveOneD::MustDownCast(aSecTwo.get())->ptrConstDtCurve(),
                polynomial,
                absShape,
                maxDegree,
                nbSections
    )
                .result());
  }
  else
    dt__throwUnexpected(buildPart());

  //
  // create vec3dSurfaceTwoD
  //
  if (dtXmlParserBase::getAttributeBool("exchange", toBuild, false))
  {
    dtS.reset(bSplineSurface_exchangeSurfaceConstructOCC(dtS.get()).result());
  }
  result->push_back(new vec3dSurfaceTwoD(dtS.get()));
}
} // namespace dtOO
