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

#include "bSplineSurface2d_bSplineCurve2dFillConstructOCCAFXmlBuilder.h"

#include <xmlHeaven/aFXmlBuilderFactory.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <geometryEngine/dtCurve2d.h>
#include <geometryEngine/dtSurface2d.h>
#include <analyticFunctionHeaven/vec2dCurve2dOneD.h>
#include <analyticFunctionHeaven/vec2dSurface2dTwoD.h>
#include <geometryEngine/geoBuilder/bSplineSurface2d_bSplineCurve2dFillConstructOCC.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <baseContainerHeaven/baseContainer.h>
#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bool bSplineSurface2d_bSplineCurve2dFillConstructOCCAFXmlBuilder::_registrated 
  =
  aFXmlBuilderFactory::registrate(
    dt__tmpPtr(
      bSplineSurface2d_bSplineCurve2dFillConstructOCCAFXmlBuilder, 
      new bSplineSurface2d_bSplineCurve2dFillConstructOCCAFXmlBuilder()
    )
  );
  
  bSplineSurface2d_bSplineCurve2dFillConstructOCCAFXmlBuilder
    ::bSplineSurface2d_bSplineCurve2dFillConstructOCCAFXmlBuilder() {
  }

  bSplineSurface2d_bSplineCurve2dFillConstructOCCAFXmlBuilder
    ::~bSplineSurface2d_bSplineCurve2dFillConstructOCCAFXmlBuilder() {

  }

  void bSplineSurface2d_bSplineCurve2dFillConstructOCCAFXmlBuilder::buildPart(
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
			vectorHandling< dtCurve2d const * > cc;
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
					vec2dCurve2dOneD const * v2d1d,
					vec2dCurve2dOneD::ConstDownCast(aF.get())
				);
				cc.push_back( v2d1d->ptrDtCurve2d()->clone() );
      }

			ptrHandling<dtSurface2d> dtS2d( 
				bSplineSurface2d_bSplineCurve2dFillConstructOCC(cc).result()
			);

      //
      // create scaCurve2dOneD
      //
      sFunP->push_back( new vec2dSurface2dTwoD( dtS2d.get() ) );			
    }
    else dt__throw(buildPart(), << dt__eval(hasFunction) );
  }
}
