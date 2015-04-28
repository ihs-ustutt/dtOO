#include "sca5PointMeanlineFunctionDecorator.h"
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
  sca5PointMeanlineFunctionDecorator::sca5PointMeanlineFunctionDecorator() { 
  }

  sca5PointMeanlineFunctionDecorator::~sca5PointMeanlineFunctionDecorator() {

  }

  void sca5PointMeanlineFunctionDecorator::buildPart(
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
    bool hasPointOne = hasAttribute("point_one", toBuildP);
    bool hasPointTwo = hasAttribute("point_two", toBuildP);

    //
    //
    //
		dt__pH(dtCurve2d) dtC2d;
    if ( hasAlphaOne && hasAlphaTwo && hasRatio && !hasDeltaX && hasDeltaY && hasOrder && hasPointOne && hasPointTwo) {
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
       float pointOne = muParseString(
                       replaceUsedFunctions(
                         getAttributeStr(
                           "point_one", 
                           toBuildP
                         ), 
                         cValP, 
                         depSFunP
                       )
                     ); 
	float pointTwo = muParseString(
                       replaceUsedFunctions(
                         getAttributeStr(
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
      dt__throw(buildPart(),
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
		if ( hasAttribute("revert", toBuildP) ) {
			if ( getAttributeBool("revert", toBuildP) ) {
				dtOCCBSplineCurve2d::SecureCast(dtC2d.get())->revert();
			}
		}		
		sFunP->push_back( new vec2dCurve2dOneD( dtC2d.get() ) );
  }
}
