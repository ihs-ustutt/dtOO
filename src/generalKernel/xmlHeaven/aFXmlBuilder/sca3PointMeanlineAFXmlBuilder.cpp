#include "sca3PointMeanlineAFXmlBuilder.h"

#include <QtXml/QDomElement>

#include <logMe/logMe.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <baseContainerHeaven/baseContainer.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <geometryEngine/dtOCCCurve2d.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_angleRatioDeltaYConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_angleDeltaXDeltaYConstructOCC.h>
#include <analyticFunctionHeaven/vec2dCurve2dOneD.h>

namespace dtOO {
  sca3PointMeanlineAFXmlBuilder::sca3PointMeanlineAFXmlBuilder() { 
  }

  sca3PointMeanlineAFXmlBuilder::~sca3PointMeanlineAFXmlBuilder() {

  }

  void sca3PointMeanlineAFXmlBuilder::buildPart(
		::QDomElement const & toBuildP, 
		baseContainer * const bC,
		vectorHandling< constValue * > const * const cValP, 
		vectorHandling< analyticFunction * > const * const depSFunP,
		vectorHandling< analyticFunction * > * sFunP 
	) const {
    dt__throwIf(
      !dtXmlParserBase::hasAttribute("alpha_one", toBuildP), buildPart()
    );
    dt__throwIf(
      !dtXmlParserBase::hasAttribute("alpha_two", toBuildP), buildPart()
    );        
    bool hasRatio = dtXmlParserBase::hasAttribute("ratio", toBuildP);
		bool hasDeltaX = dtXmlParserBase::hasAttribute("delta_x", toBuildP);
    bool hasDeltaY = dtXmlParserBase::hasAttribute("delta_y", toBuildP);

    float alphaOne 
    = 
    dtXmlParserBase::muParseString(
      dtXmlParserBase::replaceDependencies(
        dtXmlParserBase::getAttributeStr(
          "alpha_one", 
          toBuildP
        ), 
        cValP, 
        depSFunP
      )
    );      
    float alphaTwo 
    = 
    dtXmlParserBase::muParseString(
      dtXmlParserBase::replaceDependencies(
        dtXmlParserBase::getAttributeStr(
          "alpha_two", 
          toBuildP
        ), 
        cValP, 
        depSFunP
      )
    );          
    //
    //
    //
		dt__pH(dtCurve2d) dtC2d;
    if ( hasRatio && !hasDeltaX && hasDeltaY ) {
      //
      // get necessary values
      //
      float deltaY = dtXmlParserBase::muParseString(
                       dtXmlParserBase::replaceDependencies(
                         dtXmlParserBase::getAttributeStr(
                           "delta_y", 
                           toBuildP
                         ), 
                         cValP, 
                         depSFunP
                       )
                     ); 
      float ratio = dtXmlParserBase::muParseString(
                       dtXmlParserBase::replaceDependencies(
                         dtXmlParserBase::getAttributeStr(
                           "ratio", 
                           toBuildP
                         ), 
                         cValP, 
                         depSFunP
                       )
                     );      
			dtC2d.reset(
				bSplineCurve2d_angleRatioDeltaYConstructOCC(
					alphaOne, alphaTwo, ratio, deltaY
				).result()
			);
    }
    else if ( !hasRatio && hasDeltaX && hasDeltaY ) {
      //
      // get necessary values
      //
      float deltaY = dtXmlParserBase::muParseString(
                       dtXmlParserBase::replaceDependencies(
                         dtXmlParserBase::getAttributeStr(
                           "delta_y", 
                           toBuildP
                         ), 
                         cValP, 
                         depSFunP
                       )
                     ); 
      float deltaX = dtXmlParserBase::muParseString(
                       dtXmlParserBase::replaceDependencies(
                         dtXmlParserBase::getAttributeStr(
                           "delta_x", 
                           toBuildP
                         ), 
                         cValP, 
                         depSFunP
                       )
                     ); 			
			dtC2d.reset(
				bSplineCurve2d_angleDeltaXDeltaYConstructOCC(
					alphaOne, alphaTwo, deltaX, deltaY
				).result()
			);
    }		
    else dt__throwUnexpected(buildPart());
    
		if ( dtXmlParserBase::hasAttribute("revert", toBuildP) ) {
			if ( dtXmlParserBase::getAttributeBool("revert", toBuildP) ) {
				dtOCCCurve2d::SecureCast(dtC2d.get())->revert();
			}
		}		
		sFunP->push_back( new vec2dCurve2dOneD( dtC2d.get() ) );
  }
}
