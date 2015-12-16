#include "typeCast.h"
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <analyticGeometryHeaven/rotatingSpline.h>

namespace dtOO {
  DTCLASSLOGMETHODI(typeCast,
          << dt__eval(thisRef._tolerance) << std::endl
          << dt__eval(thisRef._maxStep) );
  
  typeCast::typeCast() {
  }

  typeCast::typeCast(const typeCast& orig) {
  }

  typeCast::~typeCast() {
  }
  
  vectorHandling< analyticGeometry * > typeCast::apply( vectorHandling< analyticGeometry * > const * const aGeoVecP ) const {
    vectorHandling< analyticGeometry * > retAGeo;
    
    dt__FORALL(*aGeoVecP, ii,      
      rotatingSpline * rS;
      dt__CANDOWNCAST(aGeoVecP->at(ii), rotatingSpline, rS);
			
//      if ( rS ) {
//        dt__info(apply(),
//                << "casting rotatingSpline " << rS->getLabel() << " ... ");
//        perfRotatingSpline * pRS = new perfRotatingSpline( *rS );
//        pRS->initInternalFunctions();
//        retAGeo.push_back( pRS );
//      }    
    );
    
    return retAGeo;    
  }

  bool typeCast::isNecessary( void ) const {
    return true;
  }
  
  void typeCast::init( ::QDomElement * transformerElementP, 
                   pointContainer * const pointContainerP,
                   vectorContainer * const vectorContainerP,    
                   vectorHandling< constValue * > const * const cValP,
                   vectorHandling< scaFunction * > const * const sFunP,
                   vectorHandling< analyticGeometry * > const * const depAGeoP ) {
    _tolerance = 0.01;
    _maxStep = -1.;
    
    /** @xmlAttribute{tolerance, float, Tolerance parameter to create casted analyticGeometry.} */
    if (transformerElementP->hasAttribute("tolerance")) {
      _tolerance = muParseString( replaceUsedFunctions(
                       getAttributeStr("tolerance", *transformerElementP),
                       cValP, 
                       sFunP) 
                     );
    }
    /** @xmlAttribute{max_step, float, Maximum number of steps to create approximation of splineCurve3dBoundToAnalyticSurface} */
    if (transformerElementP->hasAttribute("max_step")) {
      _maxStep = muParseString( replaceUsedFunctions(
                       getAttributeStr("max_step", *transformerElementP),
                       cValP, 
                       sFunP) 
                     );
    }     
    
  }
  
}
