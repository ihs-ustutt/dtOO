#include "vec3dTwoDTwelveRectangleFunctionDecorator.h"
#include <geometryEngine/geoBuilder/bSplineCurve_pointConstructOCC.h>
#include <geometryEngine/geoBuilder/trimmedCurve_twoPointsConnectConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve_normalOffsetGeomCurveOCC.h>
#include <analyticFunctionHeaven/vec3dSurfaceTwoD.h>
#include <geometryEngine/geoBuilder/trimmedCurve_uBounds.h>
#include <geometryEngine/geoBuilder/geomSurface_geomCurveFillConstructOCC.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtSurface.h>
#include <analyticFunctionHeaven/vec3dCurveOneD.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  vec3dTwoDTwelveRectangleFunctionDecorator::vec3dTwoDTwelveRectangleFunctionDecorator() {
  }

  vec3dTwoDTwelveRectangleFunctionDecorator::~vec3dTwoDTwelveRectangleFunctionDecorator() {

  }

  void vec3dTwoDTwelveRectangleFunctionDecorator::buildPart(
    QDomElement const & toBuildP, 
    baseContainer * const bC,					
		vectorHandling< constValue * > const * const cValP, 
		vectorHandling< analyticFunction * > const * const depSFunP,
		vectorHandling< analyticFunction * > * sFunP 
	) const {
		//
		// initial values
		//
		float tt = .02;
		std::vector< float > ll(2);
		ll[0] = .45; ll[1] = .45;
		std::vector< float > mm(2);
		mm[0] = .45; mm[1] = .45;
		std::vector< float > nn(2);
		nn[0] = .20; nn[1] = .20;		
		std::vector< float > kk(2);
		kk[0] = .20; kk[1] = .20;
		
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
				dt__ptrAss(
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
			
			//
			// add boundary layer curve
			//
			cL.push_back( bSplineCurve_normalOffsetGeomCurveOCC(cL[4], tt).result() );
			
      analyticFunction * vec3dTwoDaFC = createCompound(cL, ll, mm, nn, kk);
			cL.destroy();
			
      //
      // create vec3dSurfaceTwoD
      //
			sFunP->push_back(vec3dTwoDaFC);
			
			//
			// angle output
			//
			std::vector< float > itVal;
			std::vector< std::string > header;
			header.push_back("alpha_0_0");
			header.push_back("alpha_1_0");
			header.push_back("alpha_1_1");
			header.push_back("alpha_0_1");
			header.push_back("alpha_av");
			vectorHandling< analyticFunction const * > aFV = vec3dTwoDaFC->compoundInternal();
			for (int ii=0; ii<aFV.size(); ii++) {
				float av = 0.;
			  dt__ptrAss(
					vec3dSurfaceTwoD const * v3d2d,
				  vec3dSurfaceTwoD::ConstDownCast(aFV[ii])
				);
				std::vector< dtVector3 > DY;
				DY = v3d2d->DYdtVector3( v3d2d->x_percent(0.0, 0.0) );
				itVal.push_back( dtLinearAlgebra::angleDegree( DY[0], DY[1] ) );
				av = av + itVal.back();
				DY = v3d2d->DYdtVector3( v3d2d->x_percent(1.0, 0.0) );
				itVal.push_back( dtLinearAlgebra::angleDegree( DY[0], DY[1] ) );
				av = av + itVal.back();
				DY = v3d2d->DYdtVector3( v3d2d->x_percent(1.0, 1.0) );
				itVal.push_back( dtLinearAlgebra::angleDegree( DY[0], DY[1] ) );				
				av = av + itVal.back();
				DY = v3d2d->DYdtVector3( v3d2d->x_percent(0.0, 1.0) );
				itVal.push_back( dtLinearAlgebra::angleDegree( DY[0], DY[1] ) );				
				av = av + itVal.back();
				itVal.push_back(av/4.);
			}
			DTINFOWF(
				buildPart(),
				<< logMe::floatVecToTable(header, itVal)
			);
    }
    else {
      dt__THROW(buildPart(), << DTLOGEVAL(hasAF) );
    }
  }
	
  void vec3dTwoDTwelveRectangleFunctionDecorator::reorder( 
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

		cc.clear();
		cc.resize(5);
		cc[0] = retCC[0];
		cc[1] = retCC[1];
		cc[2] = retCC[2];
		cc[3] = retCC[3];
		cc[4] = retCC[4];      
	}    	
	
	analyticFunction * 
	vec3dTwoDTwelveRectangleFunctionDecorator::createCompound( 
	  vectorHandling< const dtCurve * > & cL,
    std::vector< float > const & ll, std::vector< float > const & mm,
    std::vector< float > const & nn, std::vector< float > const & kk 					
	) {
		//
		// create surface from four bounding curves
		//
		ptrHandling< dtSurface > dtS( 
			geomSurface_geomCurveFillConstructOCC(
				cL[0], cL[1], cL[2], cL[3]
			).result() 
		);					
		vectorHandling< dtCurve const * > cL2(32);
		cL2.nullify();

		dtPoint2 ppUV;
		cL2[14] 
		= 
		trimmedCurve_uBounds(
			cL[5], cL[5]->getUMin(), cL[5]->u_lPercent(mm[0])
		).result();
		cL2[15] 
		= 
		trimmedCurve_uBounds(
			cL[4], cL[4]->getUMin(), cL[4]->u_lPercent(ll[0])
		).result();
		cL2[10] 
		= 
		trimmedCurve_twoPointsConnectConstructOCC(
			cL2[15]->pointPercent(0.), cL2[14]->pointPercent(0.)
		).result();
		cL2[20] 
		= 
		trimmedCurve_twoPointsConnectConstructOCC(
			cL2[15]->pointPercent(1.), cL2[14]->pointPercent(1.)
		).result();
		cL2[21] 
		= 
		trimmedCurve_uBounds(
			cL[4], cL[4]->u_lPercent(ll[0]), cL[4]->u_lPercent(1.-ll[1])
		).result();			
		cL2[26] 
		= 
		trimmedCurve_uBounds(
			cL[5], cL[5]->u_lPercent(mm[0]), cL[5]->u_lPercent(1.-mm[1])
		).result();						
		cL2[22] 
		= 
		trimmedCurve_twoPointsConnectConstructOCC(
			cL2[21]->pointPercent(1.), cL2[26]->pointPercent(1.)
		).result();
		cL2[16] 
		= 
		trimmedCurve_uBounds(
			cL[4], cL[4]->u_lPercent(1.-ll[1]), cL[4]->u_lPercent(1.)
		).result();			
		cL2[17] 
		= 
		trimmedCurve_uBounds(
			cL[5], cL[5]->u_lPercent(1.-mm[1]), cL[5]->u_lPercent(1.)
		).result();						
		cL2[11] 
		= 
		trimmedCurve_twoPointsConnectConstructOCC(
			cL2[16]->pointPercent(1.), cL2[17]->pointPercent(1.)
		).result();
		ppUV = dtS->reparam(cL2[20]->pointPercent(1.));
		cL2[25] = dtS->segmentConstU(ppUV.x(), ppUV.y(), dtS->maxV());			
		ppUV = dtS->reparam(cL2[22]->pointPercent(1.));
		cL2[27] = dtS->segmentConstU(ppUV.x(), ppUV.y(), dtS->maxV());
		cL2[30] 
		= 
		trimmedCurve_twoPointsConnectConstructOCC(
			cL2[25]->pointPercent(1.), cL2[27]->pointPercent(1.)
		).result();
		ppUV = dtS->reparam(cL2[10]->pointPercent(1.));
		cL2[5] = dtS->segmentConstU(ppUV.x(), dtS->minV(), ppUV.y());
		ppUV = dtS->reparam(cL2[15]->pointPercent(0.));
		cL2[6] = dtS->segmentConstU(ppUV.x(), dtS->minV(), ppUV.y());
		ppUV = dtS->reparam(cL2[11]->pointPercent(1.));
		cL2[7] = dtS->segmentConstU(ppUV.x(), dtS->minV(), ppUV.y());
		cL2[1] 
		= 
		trimmedCurve_twoPointsConnectConstructOCC(
			cL2[5]->pointPercent(0.), cL2[6]->pointPercent(0.)
		).result();			
		cL2[2] 
		= 
		trimmedCurve_twoPointsConnectConstructOCC(
			cL2[7]->pointPercent(0.), cL2[6]->pointPercent(0.)
		).result();
		//ppUV = dtS->reparam(cL2[11]->pointPercent(1.));
		cL2[24] 
		= 
		trimmedCurve_uBounds(
			cL[3], cL[3]->getUMin(), cL[3]->u_lPercent(kk[1])
		).result();
		cL2[19] 
		=
		trimmedCurve_twoPointsConnectConstructOCC(
			cL2[24]->pointPercent(1.), cL2[17]->pointPercent(0.)
		).result();			
		cL2[29] 
		=
		trimmedCurve_twoPointsConnectConstructOCC(
			cL2[24]->pointPercent(0.), cL2[27]->pointPercent(1.)
		).result();							
		cL2[28] 
		= 
		trimmedCurve_uBounds(
			cL[1], cL[1]->u_lPercent(1.-nn[1]), cL[1]->getUMax()
		).result();
		cL2[23] 
		=
		trimmedCurve_twoPointsConnectConstructOCC(
			cL2[28]->pointPercent(0.), cL2[14]->pointPercent(1.)
		).result();			
		cL2[31] 
		=
		trimmedCurve_twoPointsConnectConstructOCC(
			cL2[28]->pointPercent(1.), cL2[25]->pointPercent(1.)
		).result();				
		cL2[4] 
		= 
		trimmedCurve_uBounds(
			cL[1], cL[1]->getUMin(), cL[1]->u_lPercent(nn[0])
		).result();
		cL2[0] 
		=
		trimmedCurve_twoPointsConnectConstructOCC(
			cL2[5]->pointPercent(0.), cL[0]->pointPercent(1.)
		).result();			
		cL2[9] 
		=
		trimmedCurve_twoPointsConnectConstructOCC(
			cL2[5]->pointPercent(1.), cL2[4]->pointPercent(1.)
		).result();				
		cL2[13] 
		= 
		trimmedCurve_uBounds(
			cL[1], cL[1]->u_lPercent(nn[0]), cL[1]->u_lPercent(1.-nn[1])
		).result();			

		cL2[8] 
		= 
		trimmedCurve_uBounds(
			cL[3], cL[3]->u_lPercent(1.-kk[0]), cL[3]->getUMax()
		).result();
		cL2[3] 
		=
		trimmedCurve_twoPointsConnectConstructOCC(
			cL2[8]->pointPercent(1.), cL2[7]->pointPercent(0.)
		).result();			
		cL2[12] 
		=
		trimmedCurve_twoPointsConnectConstructOCC(
			cL2[7]->pointPercent(1.), cL2[8]->pointPercent(0.)
		).result();				
		cL2[18] 
		= 
		trimmedCurve_uBounds(
			cL[3], cL[3]->u_lPercent(kk[1]), cL[3]->u_lPercent(1.-kk[0])
		).result();		

		vec3dSurfaceTwoDCompound * vec3dTwoDaFC 
		= 
		new vec3dSurfaceTwoDCompound();
		ptrHandling< dtSurface > dtS2;
		dtS2.reset( 
			geomSurface_geomCurveFillConstructOCC(
				cL2[1], cL2[5], cL2[10], cL2[6]
			).result() 
		);			
		vec3dTwoDaFC->addComponent( vec3dSurfaceTwoD(dtS2.get()) );
		dtS2.reset( 
			geomSurface_geomCurveFillConstructOCC(
				cL2[2], cL2[6], cL2[11], cL2[7]
			).result() 
		);			
		vec3dTwoDaFC->addComponent( vec3dSurfaceTwoD(dtS2.get()) );			
		dtS2.reset( 
			geomSurface_geomCurveFillConstructOCC(
				cL2[14], cL2[20], cL2[15], cL2[10]
			).result()
		);
		vec3dTwoDaFC->addComponent( vec3dSurfaceTwoD(dtS2.get()) );
		dtS2.reset( 
			geomSurface_geomCurveFillConstructOCC(
				cL2[26], cL2[22], cL2[21], cL2[20]
			).result() 
		);
		vec3dTwoDaFC->addComponent( vec3dSurfaceTwoD(dtS2.get()) );			
		dtS2.reset( 
			geomSurface_geomCurveFillConstructOCC(
				cL2[17], cL2[11], cL2[16], cL2[22]
			).result() 
		);
		vec3dTwoDaFC->addComponent( vec3dSurfaceTwoD(dtS2.get()) );						
		dtS2.reset( 
			geomSurface_geomCurveFillConstructOCC(
				cL2[25], cL2[30], cL2[27], cL2[26]
			).result() 
		);
		vec3dTwoDaFC->addComponent( vec3dSurfaceTwoD(dtS2.get()) );			
		dtS2.reset( 
			geomSurface_geomCurveFillConstructOCC(
				cL2[19], cL2[24], cL2[29], cL2[27]
			).result() 
		);
		vec3dTwoDaFC->addComponent( vec3dSurfaceTwoD(dtS2.get()) );			
		dtS2.reset( 
			geomSurface_geomCurveFillConstructOCC(
				cL2[23], cL2[25], cL2[31], cL2[28]
			).result() 
		);
		vec3dTwoDaFC->addComponent( vec3dSurfaceTwoD(dtS2.get()) );			
		dtS2.reset( 
			geomSurface_geomCurveFillConstructOCC(
				cL2[0], cL2[4], cL2[9], cL2[5]
			).result() 
		);
		vec3dTwoDaFC->addComponent( vec3dSurfaceTwoD(dtS2.get()) );		
		dtS2.reset( 
			geomSurface_geomCurveFillConstructOCC(
				cL2[9], cL2[13], cL2[23], cL2[14]
			).result() 
		);
		vec3dTwoDaFC->addComponent( vec3dSurfaceTwoD(dtS2.get()) );					
		dtS2.reset( 
			geomSurface_geomCurveFillConstructOCC(
				cL2[12], cL2[8], cL2[3], cL2[7]
			).result() 
		);
		vec3dTwoDaFC->addComponent( vec3dSurfaceTwoD(dtS2.get()) );														
		dtS2.reset( 
			geomSurface_geomCurveFillConstructOCC(
				cL2[19], cL2[18], cL2[12], cL2[17]
			).result() 
		);
		vec3dTwoDaFC->addComponent( vec3dSurfaceTwoD(dtS2.get()) );				
		
		cL2.destroy();
		
		return vec3dTwoDaFC;
	}
}
