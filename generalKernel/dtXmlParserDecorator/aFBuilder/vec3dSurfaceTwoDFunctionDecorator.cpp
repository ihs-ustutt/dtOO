#include "vec3dSurfaceTwoDFunctionDecorator.h"
//#include "functionHeaven/vec2dCurve2dOneD.h"
#include "geometryEngine/geoBuilder/bSplineSurface_skinConstructOCC.h"
//#include "geometryEngine/geoBuilder/bSplineSurface_bSplineCurveFillConstructOCC.h"
#include <functionHeaven/analyticFunction.h>
#include <dtTransformerHeaven/dtTransformerFactory.h>
#include <dtTransformerHeaven/dtTransformer.h>
#include <geometryEngine/geoBuilder/bSplineSurface_skinConstructOCC.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtSurface.h>
#include <functionHeaven/vec3dCurveOneD.h>
#include <functionHeaven/vec3dSurfaceTwoD.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  vec3dSurfaceTwoDFunctionDecorator::vec3dSurfaceTwoDFunctionDecorator() {
  }

  vec3dSurfaceTwoDFunctionDecorator::~vec3dSurfaceTwoDFunctionDecorator() {

  }

  void vec3dSurfaceTwoDFunctionDecorator::buildPart(
         QDomElement const & toBuildP, 
         vectorHandling< constValue * > const * const cValP, 
         vectorHandling< analyticFunction * > const * const depSFunP,
         vectorHandling< analyticFunction * > * sFunP ) const {
//	<function label="f_meanplane_phiR_m_s">
//		<builder name="vec3dSurfaceTwoDFunction"/>
//		  <analyticFunction label="f_sm_hub"/>
//			<analyticFunction label="f_sm_mplane500"/>
//			<analyticFunction label="f_sm_shroud"/>
//		<builder>
//	</function>		
    //
    //check input
    //
//    bool hasOrder = hasAttribute("order", toBuildP);
    bool hasAF = hasChild("analyticFunction", toBuildP);
//    bool hasScaFunction = hasChild("function", toBuildP);
//    
//
    if ( hasAF ) {
//			std::vector< dtPoint2 > pointsArray;
      QDomElement elementP = getChild("analyticFunction", toBuildP);
      //
      // set input
      //
//      int order = getAttributeInt("order", toBuildP);
			vectorHandling< dtCurve const * > cL;
      while ( !elementP.isNull() ) {
//				std::vector< dtPoint2* > workingPoint;
        analyticFunction const * const aF 
				= 
				createAnalyticFunction( &elementP, cValP, depSFunP );
				dt__PTRASS(
				  vec3dCurveOneD const * v3d1d, 
				  vec3dCurveOneD::ConstDownCast(aF)
				);
				cL.push_back( v3d1d->ptrDtCurve() );
//        for (int ii=0;ii<workingPoint.size();ii++) {
//          // put in point builder
//					pointsArray.push_back( *(workingPoint[ii]) );
//					delete workingPoint[ii];
//        }
//				// next sibling
				elementP = getNextSibling("analyticFunction", elementP);      
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
      dt__THROW(buildPart(),
//              << DTLOGEVAL(hasOrder) << LOGDEL
//              << DTLOGEVAL(hasPoints) << LOGDEL
              << DTLOGEVAL(hasAF) );
    }

		//
		// transform
		//
    ptrHandling< dtTransformer > cTransP(  
		  createTransformer(&toBuildP, cValP, depSFunP)
		);	
    if ( cTransP->isNecessary() ) {
      *sFunP = cTransP->apply(sFunP);
    }
  }
}
