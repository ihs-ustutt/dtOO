#include "vec3dTwoDFiveRectangleFunctionDecorator.h"
#include <geometryEngine/geoBuilder/trimmedCurve_twoPointsConnectConstructOCC.h>
#include <analyticFunctionHeaven/vec3dSurfaceTwoD.h>
#include <geometryEngine/geoBuilder/trimmedCurve_uBounds.h>
#include <geometryEngine/geoBuilder/geomSurface_geomCurveFillConstructOCC.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <dtTransformerHeaven/dtTransformerFactory.h>
#include <dtTransformerHeaven/dtTransformer.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtSurface.h>
#include <analyticFunctionHeaven/vec3dCurveOneD.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  vec3dTwoDFiveRectangleFunctionDecorator::vec3dTwoDFiveRectangleFunctionDecorator() {
  }

  vec3dTwoDFiveRectangleFunctionDecorator::~vec3dTwoDFiveRectangleFunctionDecorator() {

  }

  void vec3dTwoDFiveRectangleFunctionDecorator::buildPart(
		QDomElement const & toBuildP, 
		baseContainer * const bC,
		vectorHandling< constValue * > const * const cValP, 
		vectorHandling< analyticFunction * > const * const depSFunP,
		vectorHandling< analyticFunction * > * sFunP 
	) const {
		
		float A = .5;
		float B = .1;
		float C = .8;
		float D = .3;
		float E = .7;
		
		vec3dCurveOneDCompound * vec3dOneDaFC 
		= 
		new vec3dCurveOneDCompound();
    //
    //check input
    //
    bool hasAF = hasChild("analyticFunction", toBuildP);
    if ( hasAF ) {
      QDomElement elementP = getChild("analyticFunction", toBuildP);
      //
      // set input
      //
			vectorHandling< dtCurve const * > cL;
      while ( !elementP.isNull() ) {
        analyticFunction const * const aF 
				= 
				createAnalyticFunction( &elementP, bC, cValP, depSFunP );
				dt__PTRASS(
				  vec3dCurveOneD const * v3d1d, 
				  vec3dCurveOneD::ConstDownCast(aF)
				);
				cL.push_back( v3d1d->ptrDtCurve()->clone() );
				elementP = getNextSibling("analyticFunction", elementP);      
      }
			//
			// reorder curves
			//
			reorder(cL);
			
			
			ptrHandling<dtCurve> dtC;		
      dtC.reset( // 0
				trimmedCurve_uBounds(
			    cL[0], cL[0]->u_uPercent(0.), cL[0]->u_uPercent(A)
			  ).result() 
			);
      vec3dOneDaFC->addComponent( vec3dCurveOneD(dtC.get()) );					
      dtC.reset( // 1
				trimmedCurve_twoPointsConnectConstructOCC(
			    cL[0]->pointPercent(A), cL[4]->pointPercent(0.)
			  ).result() 
			);
      vec3dOneDaFC->addComponent( vec3dCurveOneD(dtC.get()) );		
      dtC.reset( // 2
				trimmedCurve_twoPointsConnectConstructOCC(
			    cL[4]->pointPercent(0.), cL[3]->pointPercent(C)
			  ).result() 
			);
      vec3dOneDaFC->addComponent( vec3dCurveOneD(dtC.get()) );				
      dtC.reset( // 3
				trimmedCurve_twoPointsConnectConstructOCC(
			    cL[3]->pointPercent(C), cL[3]->pointPercent(1.)
			  ).result() 
			);
      vec3dOneDaFC->addComponent( vec3dCurveOneD(dtC.get()) );			

      dtC.reset( // 4
				trimmedCurve_uBounds(
			    cL[0], cL[0]->u_uPercent(A), cL[0]->u_uPercent(1.)
			  ).result() 
			);
      vec3dOneDaFC->addComponent( vec3dCurveOneD(dtC.get()) );					
      dtC.reset( // 5
				trimmedCurve_uBounds(
			    cL[1], cL[1]->u_uPercent(0.), cL[1]->u_uPercent(B)
			  ).result() 
			);
      vec3dOneDaFC->addComponent( vec3dCurveOneD(dtC.get()) );					
      dtC.reset( // 6
				trimmedCurve_twoPointsConnectConstructOCC(
			    cL[1]->pointPercent(B), cL[4]->pointPercent(0.)
			  ).result() 
			);
      vec3dOneDaFC->addComponent( vec3dCurveOneD(dtC.get()) );					
			
      dtC.reset( // 7
				trimmedCurve_uBounds(
			    cL[1], cL[1]->u_uPercent(B), cL[1]->u_uPercent(1.)
			  ).result() 
			);
      vec3dOneDaFC->addComponent( vec3dCurveOneD(dtC.get()) );					
      dtC.reset( // 8
				trimmedCurve_twoPointsConnectConstructOCC(
			    cL[1]->pointPercent(1.), cL[4]->pointPercent(D)
			  ).result() 
			);
      vec3dOneDaFC->addComponent( vec3dCurveOneD(dtC.get()) );							
      dtC.reset( // 9
				trimmedCurve_uBounds(
			    cL[4], cL[4]->u_uPercent(0.), cL[4]->u_uPercent(D)
			  ).result() 
			);
      vec3dOneDaFC->addComponent( vec3dCurveOneD(dtC.get()) );					
			
      dtC.reset( // 10
				trimmedCurve_uBounds(
			    cL[4], cL[4]->u_uPercent(D), cL[4]->u_uPercent(E)
			  ).result() 
			);
      vec3dOneDaFC->addComponent( vec3dCurveOneD(dtC.get()) );					
      dtC.reset( // 11
				trimmedCurve_twoPointsConnectConstructOCC(
			    cL[4]->pointPercent(E), cL[2]->pointPercent(1.)
			  ).result() 
			);
      vec3dOneDaFC->addComponent( vec3dCurveOneD(dtC.get()) );							
      dtC.reset( // 12
				trimmedCurve_uBounds(
			    cL[2], cL[2]->u_uPercent(0.), cL[2]->u_uPercent(1.)
			  ).result() 
			);
      vec3dOneDaFC->addComponent( vec3dCurveOneD(dtC.get()) );				
      dtC.reset( // 13
				trimmedCurve_uBounds(
			    cL[4], cL[4]->u_uPercent(E), cL[4]->u_uPercent(1.)
			  ).result() 
			);
      vec3dOneDaFC->addComponent( vec3dCurveOneD(dtC.get()) );		
      dtC.reset( // 14
				trimmedCurve_uBounds(
			    cL[3], cL[3]->u_uPercent(0.), cL[3]->u_uPercent(C)
			  ).result() 
			);
      vec3dOneDaFC->addComponent( vec3dCurveOneD(dtC.get()) );						
			cL.destroy();
			
			vec3dSurfaceTwoDCompound * vec3dTwoDaFC 
			= 
			new vec3dSurfaceTwoDCompound();
			
			ptrHandling< dtSurface > dtS;
			cL.push_back(vec3dOneDaFC->component(0).ptrDtCurve());
			cL.push_back(vec3dOneDaFC->component(1).ptrDtCurve());
			cL.push_back(vec3dOneDaFC->component(2).ptrDtCurve());
			cL.push_back(vec3dOneDaFC->component(3).ptrDtCurve());
      dtS.reset( geomSurface_geomCurveFillConstructOCC(cL).result() );
      vec3dTwoDaFC->addComponent( vec3dSurfaceTwoD(dtS.get()) );									
			cL.clear();
			cL.push_back(vec3dOneDaFC->component(4).ptrDtCurve());
			cL.push_back(vec3dOneDaFC->component(5).ptrDtCurve());
			cL.push_back(vec3dOneDaFC->component(6).ptrDtCurve());
			cL.push_back(vec3dOneDaFC->component(1).ptrDtCurve());
      dtS.reset( geomSurface_geomCurveFillConstructOCC(cL).result() );
      vec3dTwoDaFC->addComponent( vec3dSurfaceTwoD(dtS.get()) );									
			cL.clear();
			cL.push_back(vec3dOneDaFC->component(6).ptrDtCurve());
			cL.push_back(vec3dOneDaFC->component(7).ptrDtCurve());
			cL.push_back(vec3dOneDaFC->component(8).ptrDtCurve());
			cL.push_back(vec3dOneDaFC->component(9).ptrDtCurve());
      dtS.reset( geomSurface_geomCurveFillConstructOCC(cL).result() );
      vec3dTwoDaFC->addComponent( vec3dSurfaceTwoD(dtS.get()) );												
			cL.clear();
			cL.push_back(vec3dOneDaFC->component(8).ptrDtCurve());
			cL.push_back(vec3dOneDaFC->component(12).ptrDtCurve());
			cL.push_back(vec3dOneDaFC->component(11).ptrDtCurve());
			cL.push_back(vec3dOneDaFC->component(10).ptrDtCurve());
      dtS.reset( geomSurface_geomCurveFillConstructOCC(cL).result() );
      vec3dTwoDaFC->addComponent( vec3dSurfaceTwoD(dtS.get()) );									
			cL.clear();
			cL.push_back(vec3dOneDaFC->component(14).ptrDtCurve());
			cL.push_back(vec3dOneDaFC->component(11).ptrDtCurve());
			cL.push_back(vec3dOneDaFC->component(13).ptrDtCurve());
			cL.push_back(vec3dOneDaFC->component(2).ptrDtCurve());
      dtS.reset( geomSurface_geomCurveFillConstructOCC(cL).result() );
      vec3dTwoDaFC->addComponent( vec3dSurfaceTwoD(dtS.get()) );												
			
      //
      // create vec3dSurfaceTwoD
      //
//      sFunP->push_back(vec3dOneDaFC);			
			sFunP->push_back(vec3dTwoDaFC);			
    }
    else {
      dt__THROW(buildPart(),
              << DTLOGEVAL(hasAF) );
    }
  }
	
void vec3dTwoDFiveRectangleFunctionDecorator::reorder( 
      vectorHandling< dtCurve const * > & cc
    ) {
      std::vector< dtCurve * > retCC;    
      for (int ii=0; ii<cc.size(); ii++) {
        retCC.push_back( cc[ii]->clone() );
      }  
      dtPoint3 p0 = retCC[0]->pointPercent(0.);
      dtPoint3 p1 = retCC[0]->pointPercent(1.);
      dtPoint3 q0 = retCC[1]->pointPercent(0.);
      dtPoint3 q1 = retCC[1]->pointPercent(1.);
      dtPoint3 r0 = retCC[2]->pointPercent(0.);
      dtPoint3 r1 = retCC[2]->pointPercent(1.);
      dtPoint3 s0 = retCC[3]->pointPercent(0.);
      dtPoint3 s1 = retCC[3]->pointPercent(1.);
//      dtPoint3 t0 = retCC[4]->pointPercent(.25);
//      dtPoint3 t1 = retCC[4]->pointPercent(.75);
			
      if ( dtVector3(p1-q0).squared_length() > dtVector3(p0-q0).squared_length() ) {
        retCC[0]->revert();
        p0 = retCC[0]->pointPercent(0.);
        p1 = retCC[0]->pointPercent(1.);       
      }
      if ( dtVector3(p1-q0).squared_length() > dtVector3(p1-q1).squared_length() ) {
        retCC[1]->revert();
        dtPoint3 q0 = retCC[1]->pointPercent(0.);
        dtPoint3 q1 = retCC[1]->pointPercent(1.);
      }    
      if ( dtVector3(q1-r0).squared_length() > dtVector3(q1-r1).squared_length() ) {
        retCC[2]->revert();
        dtPoint3 r0 = retCC[2]->pointPercent(0.);
        dtPoint3 r1 = retCC[2]->pointPercent(1.);
      }      
      if ( dtVector3(p0-s1).squared_length() > dtVector3(p0-s0).squared_length() ) {
        retCC[3]->revert();
//        dtPoint3 s0 = retVec[3]->pointPercent(0.);
//        dtPoint3 s1 = retVec[3]->pointPercent(1.);
      }         
      
//			cc.clear();
      cc[0] = retCC[0];
      cc[1] = retCC[1];
      cc[2] = retCC[2];
      cc[3] = retCC[3];
			cc[4] = retCC[4];
      
//      return constRetCC;
    }    	
}
