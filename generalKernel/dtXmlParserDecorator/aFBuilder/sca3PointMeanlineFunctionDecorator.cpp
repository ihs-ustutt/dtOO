#include "sca3PointMeanlineFunctionDecorator.h"
#include <functionHeaven/analyticFunction.h>
#include <geometryEngine/dtCurve2d.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_angleRatioDeltaYConstructOCC.h>
#include <functionHeaven/vec2dCurve2dOneD.h>
#include <interfaceHeaven/ptrHandling.h>
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
         vectorHandling< constValue * > const * const cValP, 
         vectorHandling< analyticFunction * > const * const depSFunP,
         vectorHandling< analyticFunction * > * sFunP ) const {
    //
    // check input
    //
    bool hasAlphaOne = hasAttribute("alpha_one", toBuildP);
    bool hasAlphaTwo = hasAttribute("alpha_two", toBuildP);
    bool hasRatio = hasAttribute("ratio", toBuildP);
    bool hasDeltaY = hasAttribute("delta_y", toBuildP);
    bool hasOrder = hasAttribute("order", toBuildP);

    //
    //
    //
    if ( hasAlphaOne && hasAlphaTwo && hasRatio && hasDeltaY && hasOrder ) {
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
			sFunP->push_back( 
			  new vec2dCurve2dOneD( 
					dt__pH(dtCurve2d)(
						bSplineCurve2d_angleRatioDeltaYConstructOCC(
			        alphaOne, alphaTwo, ratio, deltaY
			      ).result()
					).get()
			  )
			);			
    }
    else {
      dt__THROW(buildPart(),
              << DTLOGEVAL(hasOrder) << LOGDEL
              << DTLOGEVAL(hasAlphaOne) << LOGDEL
              << DTLOGEVAL(hasAlphaTwo) << LOGDEL
              << DTLOGEVAL(hasRatio) << LOGDEL
              << DTLOGEVAL(hasDeltaY) );
    }
  }
}
