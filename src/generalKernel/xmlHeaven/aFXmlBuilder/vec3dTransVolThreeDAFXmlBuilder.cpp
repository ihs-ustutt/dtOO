#include "vec3dTransVolThreeDAFXmlBuilder.h"

#include <xmlHeaven/dtXmlParserBase.h>
#include <analyticFunctionHeaven/vec3dSurfaceTwoD.h>
#include <geometryEngine/geoBuilder/bSplineSurface_skinConstructOCC.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtSurface.h>
#include <analyticFunctionHeaven/vec3dTransVolThreeD.h>
#include <analyticFunctionHeaven/vec3dSurfaceTwoD.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  vec3dTransVolThreeDAFXmlBuilder::vec3dTransVolThreeDAFXmlBuilder() {
  }

  vec3dTransVolThreeDAFXmlBuilder::~vec3dTransVolThreeDAFXmlBuilder() {

  }

  void vec3dTransVolThreeDAFXmlBuilder::buildPart(
		::QDomElement const & toBuildP, 
		baseContainer * const bC,
		vectorHandling< constValue * > const * const cValP, 
		vectorHandling< analyticFunction * > const * const depSFunP,
		vectorHandling< analyticFunction * > * sFunP 
	) const {
    dt__throwIf(
      !dtXmlParserBase::hasChild("analyticFunction", toBuildP),
      buildPart()
    );
    
    std::vector< ::QDomElement > eVec 
    = 
    dtXmlParserBase::getChildVector("analyticFunction", toBuildP);
    
    //
    // set input
    //
    vectorHandling< analyticFunction const * > aF(eVec.size());
    dt__forAllIndex(eVec, ii) {
      aF[ii]
      =
      dtXmlParserBase::createAnalyticFunction(eVec[ii], bC, cValP, depSFunP);
    }

    vectorHandling< dtSurface const * > sL;
    dt__forAllIndex(aF, ii) {
      dt__ptrAss(
        vec3dSurfaceTwoD const * v3d2d, 
        vec3dSurfaceTwoD::ConstDownCast(aF[ii])
      );
      sL.push_back( v3d2d->ptrDtSurface() );
    }
    
    vectorHandling< dtCurve const * > cL0;
    vectorHandling< dtCurve const * > cL1;
    vectorHandling< dtCurve const * > cL2;
    vectorHandling< dtCurve const * > cL3;
    dt__forAllIndex(sL, ii) {
      cL0.push_back( sL[ii]->segmentConstVPercent(0., 0., 1.) );
      cL1.push_back( sL[ii]->segmentConstUPercent(1., 0., 1.) );
      cL2.push_back( sL[ii]->segmentConstVPercent(1., 0., 1.) );
      cL3.push_back( sL[ii]->segmentConstUPercent(0., 0., 1.) );
    }
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

	/**
	 * 
   * @todo Is this a memory bug? Should cL0 - cL4 be destroyed at the end
	 *       of the function?
   */
  void vec3dTransVolThreeDAFXmlBuilder::buildPartCompound(
	  ::QDomElement const & toBuildP, 
	  baseContainer * const bC,
    vectorHandling< constValue * > const * const cValP, 
		vectorHandling< analyticFunction * > const * const depSFunP,
		vectorHandling< analyticFunction * > * sFunP
	) const {
    dt__throwIf(
      !dtXmlParserBase::hasChild("analyticFunction", toBuildP), 
      buildPartCompound()
    );
    
    ::QDomElement elementP 
    = 
    dtXmlParserBase::getChild("analyticFunction", toBuildP);
    //
    // set input
    //
    vectorHandling< vec3dSurfaceTwoDCompound const * > v3dSC;
    while ( !elementP.isNull() ) {
      analyticFunction * aF 
      = 
      dtXmlParserBase::createAnalyticFunction( 
        &elementP, bC, cValP, depSFunP 
      );
      if ( !vec3dSurfaceTwoDCompound::ConstDownCast(aF) ) {
        dt__throw(
          buildPartCompound(), 
          << dt__eval(
            vec3dSurfaceTwoDCompound::ConstDownCast(aF)
          ) << std::endl
          << dtXmlParserBase::getAttributeStr("label", elementP) 
          << " has not correct type." 
        );
      }				
      v3dSC.push_back( vec3dSurfaceTwoDCompound::ConstSecureCast(aF) );
      elementP 
      = 
      dtXmlParserBase::getNextSibling("analyticFunction", elementP);      
    }

    int nComponents = v3dSC[0]->nComponents();
    vec3dTransVolThreeDCompound * vec3dTVC 
    =
    new vec3dTransVolThreeDCompound();
    for (int cc=0; cc<nComponents; cc++) {
      vectorHandling< dtSurface const * > sL;
      dt__forAllIndex(v3dSC, ii) {
        sL.push_back( v3dSC[ii]->component(cc).ptrDtSurface() );
      }
      vectorHandling< dtCurve const * > cL0;
      vectorHandling< dtCurve const * > cL1;
      vectorHandling< dtCurve const * > cL2;
      vectorHandling< dtCurve const * > cL3;
      dt__forAllIndex(sL, ii) {
        cL0.push_back( sL[ii]->segmentConstVPercent(0., 0., 1.) );
        cL1.push_back( sL[ii]->segmentConstUPercent(1., 0., 1.) );
        cL2.push_back( sL[ii]->segmentConstVPercent(1., 0., 1.) );
        cL3.push_back( sL[ii]->segmentConstUPercent(0., 0., 1.) );
      }
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
}
