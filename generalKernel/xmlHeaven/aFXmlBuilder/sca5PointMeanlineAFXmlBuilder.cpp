#include "sca5PointMeanlineAFXmlBuilder.h"

#include <xmlHeaven/dtXmlParserBase.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <geometryEngine/dtCurve2d.h>
#include <geometryEngine/dtOCCBSplineCurve2d.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_angleRatioDeltaYPointOnePointTwoConstructOCC.h>
#include <analyticFunctionHeaven/vec2dCurve2dOneD.h>
#include <interfaceHeaven/ptrHandling.h>
#include <baseContainerHeaven/baseContainer.h>
#include <logMe/logMe.h>
#include <math.h>
#include <QtXml/QDomElement>

namespace dtOO {
  sca5PointMeanlineAFXmlBuilder::sca5PointMeanlineAFXmlBuilder() { 
  }

  sca5PointMeanlineAFXmlBuilder::~sca5PointMeanlineAFXmlBuilder() {

  }

  void sca5PointMeanlineAFXmlBuilder::buildPart(
		QDomElement const & toBuildP, 
		baseContainer * const bC,
		vectorHandling< constValue * > const * const cValP, 
		vectorHandling< analyticFunction * > const * const depSFunP,
      vectorHandling< analyticFunction * > * sFunP 
		
	) const {
    //
    // check input
    //
    bool hasAlphaOne = dtXmlParserBase::hasAttribute("alpha_one", toBuildP);
    bool hasAlphaTwo = dtXmlParserBase::hasAttribute("alpha_two", toBuildP);
    bool hasRatio = dtXmlParserBase::hasAttribute("ratio", toBuildP);
		bool hasDeltaX = dtXmlParserBase::hasAttribute("delta_x", toBuildP);
    bool hasDeltaY = dtXmlParserBase::hasAttribute("delta_y", toBuildP);
    bool hasOrder = dtXmlParserBase::hasAttribute("order", toBuildP);	
    bool hasPointOne = dtXmlParserBase::hasAttribute("point_one", toBuildP);
    bool hasPointTwo = dtXmlParserBase::hasAttribute("point_two", toBuildP);

    //
    //
    //
		dt__pH(dtCurve2d) dtC2d;
    if (    hasAlphaOne && hasAlphaTwo 
				 && hasRatio && !hasDeltaX && hasDeltaY 
				 && hasOrder && hasPointOne && hasPointTwo
		) {
      //
      // get necessary values
      //
      float alphaOne 
			= 
			dtXmlParserBase::muParseString(
				dtXmlParserBase::replaceUsedFunctions(
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
				dtXmlParserBase::replaceUsedFunctions(
					dtXmlParserBase::getAttributeStr(
						"alpha_two", 
						toBuildP
					), 
					cValP, 
					depSFunP
				)
			);
      float deltaY 
			= 
			dtXmlParserBase::muParseString(
				dtXmlParserBase::replaceUsedFunctions(
					dtXmlParserBase::getAttributeStr(
						"delta_y", 
						toBuildP
					), 
					cValP, 
					depSFunP
				)
			); 
      float ratio
			= 
			dtXmlParserBase::muParseString(
				dtXmlParserBase::replaceUsedFunctions(
					dtXmlParserBase::getAttributeStr(
						"ratio", 
						toBuildP
					), 
					cValP, 
					depSFunP
				)
			);   
      float pointOne 
			= 
			dtXmlParserBase::muParseString(
				dtXmlParserBase::replaceUsedFunctions(
					dtXmlParserBase::getAttributeStr(
						"point_one", 
						toBuildP
					), 
					cValP, 
					depSFunP
				)
			); 
	    float pointTwo 
			= 
			dtXmlParserBase::muParseString(
				dtXmlParserBase::replaceUsedFunctions(
					dtXmlParserBase::getAttributeStr(
						"point_two", 
						toBuildP
					), 
					cValP, 
					depSFunP
				)
			); 
			dtC2d.reset(
				bSplineCurve2d_angleRatioDeltaYPointOnePointTwoConstructOCC(
					alphaOne, alphaTwo, ratio, deltaY, pointOne, pointTwo
				).result()
			);
    }
    
    else {
      dt__throw(
				buildPart(),
				<< dt__eval(hasOrder) << std::endl
				<< dt__eval(hasAlphaOne) << std::endl
				<< dt__eval(hasAlphaTwo) << std::endl
				<< dt__eval(hasRatio) << std::endl
				<< dt__eval(hasDeltaX) << std::endl
				<< dt__eval(hasDeltaY) << std::endl
				<< dt__eval(hasPointOne) << std::endl	       
				<< dt__eval(hasPointTwo)
	    );
    }
		if ( dtXmlParserBase::hasAttribute("revert", toBuildP) ) {
			if ( dtXmlParserBase::getAttributeBool("revert", toBuildP) ) {
				dtOCCBSplineCurve2d::SecureCast(dtC2d.get())->revert();
			}
		}
		sFunP->push_back( 
		  new vec2dCurve2dOneD( dtC2d.get() ) 
		);
  }
}
