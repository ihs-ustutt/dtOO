#include "vec3dSurfaceTwoDAFXmlBuilder.h"

#include <dtXmlParserDecorator/dtXmlParserBase.h>
#include "geometryEngine/geoBuilder/bSplineSurface_skinConstructOCC.h"
#include <analyticFunctionHeaven/analyticFunction.h>
#include <geometryEngine/geoBuilder/bSplineSurface_skinConstructOCC.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtSurface.h>
#include <analyticFunctionHeaven/vec3dCurveOneD.h>
#include <analyticFunctionHeaven/vec3dSurfaceTwoD.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  vec3dSurfaceTwoDAFXmlBuilder::vec3dSurfaceTwoDAFXmlBuilder() {
  }

  vec3dSurfaceTwoDAFXmlBuilder::~vec3dSurfaceTwoDAFXmlBuilder() {

  }

  void vec3dSurfaceTwoDAFXmlBuilder::buildPart(
		QDomElement const & toBuildP, 
		baseContainer * const bC,
		vectorHandling< constValue * > const * const cValP, 
		vectorHandling< analyticFunction * > const * const depSFunP,
		vectorHandling< analyticFunction * > * sFunP 
	) const {
    //
    //check input
    //
    bool hasAF = dtXmlParserBase::hasChild("analyticFunction", toBuildP);

    if ( hasAF ) {
      QDomElement elementP = dtXmlParserBase::getChild("analyticFunction", toBuildP);
      //
      // set input
      //
			vectorHandling< dtCurve const * > cL;
      while ( !elementP.isNull() ) {
        analyticFunction const * const aF 
				= 
				dtXmlParserBase::createAnalyticFunction( &elementP, bC, cValP, depSFunP );
				dt__ptrAss(
				  vec3dCurveOneD const * v3d1d, 
				  vec3dCurveOneD::ConstDownCast(aF)
				);
				cL.push_back( v3d1d->ptrDtCurve() );
				elementP = dtXmlParserBase::getNextSibling("analyticFunction", elementP);      
      }
			ptrHandling<dtSurface> dtS( 
				bSplineSurface_skinConstructOCC(cL).result() 
			);

      //
      // create vec3dSurfaceTwoD
      //
      sFunP->push_back( new vec3dSurfaceTwoD( dtS.get() ) );			
    }
    else {
      dt__throw(
			  buildPart(),
        << dt__eval(hasAF) 
			);
    }
  }
}
