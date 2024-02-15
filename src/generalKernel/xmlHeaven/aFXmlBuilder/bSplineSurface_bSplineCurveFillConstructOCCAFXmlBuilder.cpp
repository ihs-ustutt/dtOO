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

#include "bSplineSurface_bSplineCurveFillConstructOCCAFXmlBuilder.h"

#include <xmlHeaven/aFXmlBuilderFactory.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtSurface.h>
#include <analyticFunctionHeaven/vec3dCurveOneD.h>
#include <analyticFunctionHeaven/vec3dSurfaceTwoD.h>
#include <geometryEngine/geoBuilder/bSplineSurface_bSplineCurveFillConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineSurface_exchangeSurfaceConstructOCC.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <baseContainerHeaven/baseContainer.h>
#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bool bSplineSurface_bSplineCurveFillConstructOCCAFXmlBuilder::_registrated 
  =
  aFXmlBuilderFactory::registrate(
    dt__tmpPtr(
      bSplineSurface_bSplineCurveFillConstructOCCAFXmlBuilder, 
      new bSplineSurface_bSplineCurveFillConstructOCCAFXmlBuilder()
    )
  );
  
  bSplineSurface_bSplineCurveFillConstructOCCAFXmlBuilder
    ::bSplineSurface_bSplineCurveFillConstructOCCAFXmlBuilder() {
  }

  bSplineSurface_bSplineCurveFillConstructOCCAFXmlBuilder
    ::~bSplineSurface_bSplineCurveFillConstructOCCAFXmlBuilder() {

  }

  void bSplineSurface_bSplineCurveFillConstructOCCAFXmlBuilder::buildPart(
		::QDomElement const & toBuildP, 
		baseContainer * const bC,
		lvH_constValue const * const cValP, 
		lvH_analyticFunction const * const depSFunP,
		lvH_analyticFunction * sFunP 
	) const {
    //
    //check input
    //
    bool hasFunction = dtXmlParserBase::hasChild("analyticFunction", toBuildP);
    
    if ( hasFunction ) {
			vectorHandling< dtCurve const * > cc;
      std::vector< ::QDomElement > elVec 
			= 
			dtXmlParserBase::getChildVector("analyticFunction", toBuildP);
      dt__forAllIndex(elVec, ii) {
        dt__pH(analyticFunction) aF(
				  dtXmlParserBase::createAnalyticFunction(
            &elVec[ii], bC, cValP, depSFunP
          )
				);
				dt__ptrAss(
					vec3dCurveOneD const * v3d1d,
					vec3dCurveOneD::ConstDownCast(aF.get())
				);
				cc.push_back( v3d1d->ptrDtCurve()->clone() );
      }

			ptrHandling<dtSurface> dtS( 
				bSplineSurface_bSplineCurveFillConstructOCC(cc).result()
			);

      if ( dtXmlParserBase::hasAttribute("orientation", toBuildP) ) {
        dt__forAllRefAuto(
          dtXmlParserBase::getAttributeStrVector(
            "orientation", toBuildP
          ), 
          aOrient
        ) {
          if (aOrient == "exchange") {
            dtS.reset( 
              bSplineSurface_exchangeSurfaceConstructOCC(dtS.get()).result() 
            );
          }
        }
      }
      
      //
      // create scaCurve2dOneD
      //
      sFunP->push_back( new vec3dSurfaceTwoD( dtS.get() ) );			
    }
    else dt__throw(buildPart(), << dt__eval(hasFunction) );
  }
}
