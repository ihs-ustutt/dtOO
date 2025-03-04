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

#include "bSplineSurface_constrainedFillingConstructOCCAFXmlBuilder.h"

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/vec3dCurveOneD.h>
#include <analyticFunctionHeaven/vec3dSurfaceTwoD.h>
#include <baseContainerHeaven/baseContainer.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/geoBuilder/bSplineSurface_constrainedFillingConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineSurface_exchangeSurfaceConstructOCC.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/logMe.h>
#include <xmlHeaven/aFXmlBuilderFactory.h>
#include <xmlHeaven/dtXmlParserBase.h>

namespace dtOO {
bool bSplineSurface_constrainedFillingConstructOCCAFXmlBuilder::_registrated =
  aFXmlBuilderFactory::registrate(dt__tmpPtr(
    bSplineSurface_constrainedFillingConstructOCCAFXmlBuilder,
    new bSplineSurface_constrainedFillingConstructOCCAFXmlBuilder()
  ));

bSplineSurface_constrainedFillingConstructOCCAFXmlBuilder ::
  bSplineSurface_constrainedFillingConstructOCCAFXmlBuilder()
{
}

bSplineSurface_constrainedFillingConstructOCCAFXmlBuilder ::
  ~bSplineSurface_constrainedFillingConstructOCCAFXmlBuilder()
{
}

void bSplineSurface_constrainedFillingConstructOCCAFXmlBuilder::buildPart(
  ::QDomElement const &toBuild,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticFunction *result
) const
{
  dt__throwIf(
    !dtXmlParserBase::hasChild("analyticFunction", toBuild) ||
      !dtXmlParserBase::hasAttribute("maxSeg", toBuild) ||
      !dtXmlParserBase::hasAttribute("maxDeg", toBuild),
    buildPart()
  );

  vectorHandling<dtCurve const *> cc;
  std::vector<::QDomElement> elVec =
    dtXmlParserBase::getChildVector("analyticFunction", toBuild);
  dt__forAllIndex(elVec, ii)
  {
    dt__pH(analyticFunction)
      theF(dtXmlParserBase::createAnalyticFunction(&elVec[ii], bC, cV, aF));
    dt__ptrAss(
      vec3dCurveOneD const *v3d1d, vec3dCurveOneD::ConstDownCast(theF.get())
    );
    cc.push_back(v3d1d->ptrDtCurve()->clone());
  }
  dt__throwIf(cc.size() != 4, buildPart());

  ptrHandling<dtSurface> dtS(
    bSplineSurface_constrainedFillingConstructOCC(
      cc[0],
      cc[1],
      cc[2],
      cc[3],
      dtXmlParserBase::getAttributeInt("maxDeg", toBuild),
      dtXmlParserBase::getAttributeInt("maxSeg", toBuild)
    )
      .result()
  );

  if (dtXmlParserBase::hasAttribute("orientation", toBuild))
  {
    dt__forAllRefAuto(
      dtXmlParserBase::getAttributeStrVector("orientation", toBuild), aOrient
    )
    {
      if (aOrient == "exchange")
      {
        dtS.reset(bSplineSurface_exchangeSurfaceConstructOCC(dtS.get()).result()
        );
      }
    }
  }

  //
  // create scaCurve2dOneD
  //
  result->push_back(new vec3dSurfaceTwoD(dtS.get()));
}
} // namespace dtOO
