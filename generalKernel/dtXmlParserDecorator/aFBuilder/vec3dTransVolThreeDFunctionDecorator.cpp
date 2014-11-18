#include "vec3dTransVolThreeDFunctionDecorator.h"
#include <functionHeaven/vec3dSurfaceTwoD.h>
#include <geometryEngine/geoBuilder/bSplineSurface_skinConstructOCC.h>
#include <functionHeaven/analyticFunction.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtSurface.h>
#include <functionHeaven/vec3dTransVolThreeD.h>
#include <functionHeaven/vec3dSurfaceTwoD.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  vec3dTransVolThreeDFunctionDecorator::vec3dTransVolThreeDFunctionDecorator() {
  }

  vec3dTransVolThreeDFunctionDecorator::~vec3dTransVolThreeDFunctionDecorator() {

  }

  void vec3dTransVolThreeDFunctionDecorator::buildPart(
         QDomElement const & toBuildP, 
         vectorHandling< constValue * > const * const cValP, 
         vectorHandling< analyticFunction * > const * const depSFunP,
         vectorHandling< analyticFunction * > * sFunP ) const {
    //
    //check input
    //
    bool hasAF = hasChild("analyticFunction", toBuildP);
    

    if ( hasAF ) {
      QDomElement elementP = getChild("analyticFunction", toBuildP);
      //
      // set input
      //
			vectorHandling< analyticFunction const * > aF;
			while ( !elementP.isNull() ) {
        aF.push_back( createAnalyticFunction( &elementP, cValP, depSFunP ) );
				elementP = getNextSibling("analyticFunction", elementP);      
      }
			
			vectorHandling< dtSurface const * > sL;
      //dt__FORALL(aF, ii,
			for (int ii=0; ii<aF.size(); ii++) {
//							DTINFOWF(buildPart(), 
//							<< DTLOGEVAL(vec3dSurfaceTwoDCompound::ConstDownCast(aF[ii])) );
				dt__PTRASS(
				  vec3dSurfaceTwoD const * v3d2d, 
				  vec3dSurfaceTwoD::ConstDownCast(aF[ii])
				);
				sL.push_back( v3d2d->ptrDtSurface() );
      }//);
			vectorHandling< dtCurve const * > cL0;
			vectorHandling< dtCurve const * > cL1;
			vectorHandling< dtCurve const * > cL2;
			vectorHandling< dtCurve const * > cL3;
			dt__FORALL(sL, ii,
				cL0.push_back( sL[ii]->getCurveConstVPercent(0., 0., 1.) );
			  cL1.push_back( sL[ii]->getCurveConstUPercent(1., 0., 1.) );
			  cL2.push_back( sL[ii]->getCurveConstVPercent(1., 0., 1.) );
			  cL3.push_back( sL[ii]->getCurveConstUPercent(0., 0., 1.) );
		  );
			ptrHandling< dtSurface > sL0(
			  bSplineSurface_skinConstructOCC(cL0).result() 
			);
			ptrHandling< dtSurface > sL1(
			  bSplineSurface_skinConstructOCC(cL1).result() 
			);
			ptrHandling< dtSurface > sL2(
			  bSplineSurface_skinConstructOCC(cL2).result() 
			);
			ptrHandling< dtSurface > sL3(
			  bSplineSurface_skinConstructOCC(cL3).result() 
			);			

			ptrHandling<vec3dSurfaceTwoD> v3d2d0(new vec3dSurfaceTwoD(sL.back()));
			ptrHandling<vec3dSurfaceTwoD> v3d2d1(new vec3dSurfaceTwoD(sL3.get()));
			ptrHandling<vec3dSurfaceTwoD> v3d2d2(new vec3dSurfaceTwoD(sL.front()));
			ptrHandling<vec3dSurfaceTwoD> v3d2d3(new vec3dSurfaceTwoD(sL1.get()));
			ptrHandling<vec3dSurfaceTwoD> v3d2d4(new vec3dSurfaceTwoD(sL0.get()));
			ptrHandling<vec3dSurfaceTwoD> v3d2d5(new vec3dSurfaceTwoD(sL2.get()));
			
      //
      // create vec3dSurfaceTwoD
      //
			vec3dThreeD * v3d3d = 
			new vec3dTransVolThreeD( 
			  v3d2d0.get(), v3d2d1.get(), v3d2d2.get(), 
				v3d2d3.get(), v3d2d4.get(), v3d2d5.get()
			);
			sFunP->push_back(v3d3d);
    }
    else {
      dt__THROW(buildPart(),
              << DTLOGEVAL(hasAF) );
    }
  }

  void vec3dTransVolThreeDFunctionDecorator::buildPartCompound(
	  QDomElement const & toBuildP, 
    vectorHandling< constValue * > const * const cValP, 
		vectorHandling< analyticFunction * > const * const depSFunP,
		vectorHandling< analyticFunction * > * sFunP
	) const {
    //
    //check input
    //
    bool hasAF = hasChild("analyticFunction", toBuildP);
    

    if ( hasAF ) {
      QDomElement elementP = getChild("analyticFunction", toBuildP);
      //
      // set input
      //
			vectorHandling< vec3dSurfaceTwoDCompound const * > v3dSC;
			while ( !elementP.isNull() ) {
        analyticFunction * aF 
				= 
				createAnalyticFunction( &elementP, cValP, depSFunP );
				if ( !vec3dSurfaceTwoDCompound::ConstDownCast(aF) ) {
					dt__THROW(
					  buildPartCompound(), 
						<< DTLOGEVAL(vec3dSurfaceTwoDCompound::ConstDownCast(aF)) << LOGDEL
						<< getAttributeStr("label", elementP) << " has not correct type." 
					);
				}				
				v3dSC.push_back( vec3dSurfaceTwoDCompound::ConstSecureCast(aF) );
				elementP = getNextSibling("analyticFunction", elementP);      
      }
			
			int nComponents = v3dSC[0]->nComponents();
			vec3dTransVolThreeDCompound * vec3dTVC 
			=
			new vec3dTransVolThreeDCompound();
			for (int cc=0; cc<nComponents; cc++) {
				vectorHandling< dtSurface const * > sL;
				dt__FORALL(v3dSC, ii,
					sL.push_back( v3dSC[ii]->component(cc).ptrDtSurface() );
				);
				vectorHandling< dtCurve const * > cL0;
				vectorHandling< dtCurve const * > cL1;
				vectorHandling< dtCurve const * > cL2;
				vectorHandling< dtCurve const * > cL3;
				dt__FORALL(sL, ii,
					cL0.push_back( sL[ii]->getCurveConstVPercent(0., 0., 1.) );
					cL1.push_back( sL[ii]->getCurveConstUPercent(1., 0., 1.) );
					cL2.push_back( sL[ii]->getCurveConstVPercent(1., 0., 1.) );
					cL3.push_back( sL[ii]->getCurveConstUPercent(0., 0., 1.) );
				);
				ptrHandling< dtSurface > sL0(
					bSplineSurface_skinConstructOCC(cL0).result() 
				);
				ptrHandling< dtSurface > sL1(
					bSplineSurface_skinConstructOCC(cL1).result() 
				);
				ptrHandling< dtSurface > sL2(
					bSplineSurface_skinConstructOCC(cL2).result() 
				);
				ptrHandling< dtSurface > sL3(
					bSplineSurface_skinConstructOCC(cL3).result() 
				);			

				ptrHandling<vec3dSurfaceTwoD> v3d2d0(new vec3dSurfaceTwoD(sL.back()));
				ptrHandling<vec3dSurfaceTwoD> v3d2d1(new vec3dSurfaceTwoD(sL3.get()));
				ptrHandling<vec3dSurfaceTwoD> v3d2d2(new vec3dSurfaceTwoD(sL.front()));
				ptrHandling<vec3dSurfaceTwoD> v3d2d3(new vec3dSurfaceTwoD(sL1.get()));
				ptrHandling<vec3dSurfaceTwoD> v3d2d4(new vec3dSurfaceTwoD(sL0.get()));
				ptrHandling<vec3dSurfaceTwoD> v3d2d5(new vec3dSurfaceTwoD(sL2.get()));

				//
				// create vec3dSurfaceTwoD
				//
				vec3dTVC->addComponent( 
					vec3dTransVolThreeD( 
						v3d2d0.get(), v3d2d1.get(), v3d2d2.get(), 
						v3d2d3.get(), v3d2d4.get(), v3d2d5.get()
					)
				);				
			}
			
			sFunP->push_back(vec3dTVC);
    }
    else {
      dt__THROW(buildPart(), << DTLOGEVAL(hasAF) );
    }
  }	
}
