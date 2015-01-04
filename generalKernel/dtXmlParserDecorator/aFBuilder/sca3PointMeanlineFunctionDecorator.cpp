#include "sca3PointMeanlineFunctionDecorator.h"
#include <functionHeaven/analyticFunction.h>
#include <geometryEngine/dtCurve2d.h>
#include <geometryEngine/dtOCCBSplineCurve2d.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_angleRatioDeltaYConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_angleDeltaXDeltaYConstructOCC.h>
#include <functionHeaven/vec2dCurve2dOneD.h>
#include <interfaceHeaven/ptrHandling.h>
#include <baseContainerHeaven/baseContainer.h>
#include <logMe/logMe.h>
#include <math.h>
#include <QtXml/QDomElement>

namespace dtOO {
  sca3PointMeanlineFunctionDecorator::sca3PointMeanlineFunctionDecorator() { 
  }

  sca3PointMeanlineFunctionDecorator::~sca3PointMeanlineFunctionDecorator() {

  }

  void sca3PointMeanlineFunctionDecorator::buildPart(
		QDomElement const & toBuildP, 
		baseContainer * const bC,
		vectorHandling< constValue * > const * const cValP, 
		vectorHandling< analyticFunction * > const * const depSFunP,
		vectorHandling< analyticFunction * > * sFunP 
	) const {
    //
    // check input
    //
    bool hasAlphaOne = hasAttribute("alpha_one", toBuildP);
    bool hasAlphaTwo = hasAttribute("alpha_two", toBuildP);
    bool hasRatio = hasAttribute("ratio", toBuildP);
		bool hasDeltaX = hasAttribute("delta_x", toBuildP);
    bool hasDeltaY = hasAttribute("delta_y", toBuildP);
    bool hasOrder = hasAttribute("order", toBuildP);		

    //
    //
    //
		dt__pH(dtCurve2d) dtC2d;
    if ( hasAlphaOne && hasAlphaTwo && hasRatio && !hasDeltaX && hasDeltaY && hasOrder ) {
      //
      // get necessary values
      //
      float alphaOne = muParseString(
                         replaceUsedFunctions(
                           getAttributeStr(
                             "alpha_one", 
                             toBuildP
                           ), 
                           cValP, 
                           depSFunP
                         )
                       );      
      float alphaTwo = muParseString(
                         replaceUsedFunctions(
                           getAttributeStr(
                             "alpha_two", 
                             toBuildP
                           ), 
                           cValP, 
                           depSFunP
                         )
                       );
      float deltaY = muParseString(
                       replaceUsedFunctions(
                         getAttributeStr(
                           "delta_y", 
                           toBuildP
                         ), 
                         cValP, 
                         depSFunP
                       )
                     ); 
      float ratio = muParseString(
                       replaceUsedFunctions(
                         getAttributeStr(
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
    else if ( hasAlphaOne && hasAlphaTwo && !hasRatio && hasDeltaX && hasDeltaY && hasOrder ) {
      //
      // get necessary values
      //
      float alphaOne = muParseString(
                         replaceUsedFunctions(
                           getAttributeStr(
                             "alpha_one", 
                             toBuildP
                           ), 
                           cValP, 
                           depSFunP
                         )
                       );      
      float alphaTwo = muParseString(
                         replaceUsedFunctions(
                           getAttributeStr(
                             "alpha_two", 
                             toBuildP
                           ), 
                           cValP, 
                           depSFunP
                         )
                       );
      float deltaY = muParseString(
                       replaceUsedFunctions(
                         getAttributeStr(
                           "delta_y", 
                           toBuildP
                         ), 
                         cValP, 
                         depSFunP
                       )
                     ); 
      float deltaX = muParseString(
                       replaceUsedFunctions(
                         getAttributeStr(
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
    else {
      dt__THROW(buildPart(),
              << DTLOGEVAL(hasOrder) << LOGDEL
              << DTLOGEVAL(hasAlphaOne) << LOGDEL
              << DTLOGEVAL(hasAlphaTwo) << LOGDEL
              << DTLOGEVAL(hasRatio) << LOGDEL
							<< DTLOGEVAL(hasDeltaX) << LOGDEL
              << DTLOGEVAL(hasDeltaY) );
    }
		if ( hasAttribute("revert", toBuildP) ) {
			if ( getAttributeBool("revert", toBuildP) ) {
				dtOCCBSplineCurve2d::SecureCast(dtC2d.get())->revert();
			}
		}		
		sFunP->push_back( new vec2dCurve2dOneD( dtC2d.get() ) );
  }
}
