#include "dtTransformerFactory.h"
#include <logMe/logMe.h>
#include "conformalMapping.h"
#include "dtTransformer.h"
#include "doNothing.h"
#include "thicknessIncreasing.h"
#include "biThicknessIncreasing.h"
#include "offset.h"
#include "translate.h"
#include "rotate.h"
#include "conformalMapping.h"
#include "pickMap3dTo3dRangePercent.h"
#include "pickMap2dTo3dRangePercent.h"
#include "conformalMappingStartFromPoint.h"
#include "makePolynomial.h"
#include "predefinedExtension.h"
#include "closeGaps.h"
#include "pickLengthRange.h"
#include "pickLengthPercentRange.h"
#include "addConstCoordinate.h"
#include "uVw_phirMs.h"
#include "uVw_skewPhirMs.h"
#include "uVw_phiMs.h"
#include "averagePoints.h"
#include "projectOnSurface.h"
#include "reparamInSurface.h"
#include "pickVec3dTwoDRangePercent.h"
#include "analyticAddNormal.h"
#include "discreteAddNormal.h"
#include "approxInSurface.h"
#include "normalOffsetInSurface.h"
#include "closeGapsArithmetic.h"
#include "scale.h"
#include "xYz_rPhiZ.h"

#define __IFRET(className) \
    if ( strcmp(str, #className) == 0 ) { \
      return new className; \
    }
#define __IFRETCUSTOM(className, retClass) \
  if ( strcmp(str, #className) == 0 ) { \
    return new retClass; \
  }

namespace dtOO {
  dtTransformerFactory::dtTransformerFactory() {
  }

  dtTransformerFactory::~dtTransformerFactory() {
  }

  dtTransformer* dtTransformerFactory::create(char const * const str) {
    dt__info(create(), << "creating " << str <<  "...");
    
    __IFRET( doNothing );
    __IFRET( conformalMapping );
    __IFRET( thicknessIncreasing );
		__IFRET( biThicknessIncreasing );
    __IFRET( offset );
		__IFRET( translate );
    __IFRET( rotate );
    __IFRET( pickMap3dTo3dRangePercent );
		__IFRET( pickMap2dTo3dRangePercent );
    __IFRET( conformalMappingStartFromPoint );
    __IFRET( makePolynomial );
    __IFRET( predefinedExtension );
    __IFRET( closeGaps );
		__IFRET( pickLengthRange );
		__IFRET( pickLengthPercentRange );
		__IFRET( addConstCoordinate );
		__IFRET( uVw_phirMs );
    __IFRET( uVw_phiMs );
		__IFRET( averagePoints );
		__IFRET( projectOnSurface );
		__IFRET( reparamInSurface );
		__IFRET( pickVec3dTwoDRangePercent );
		__IFRET( analyticAddNormal );
    __IFRET( discreteAddNormal );
		__IFRET( approxInSurface );
		__IFRET( normalOffsetInSurface );
		__IFRET( closeGapsArithmetic );
    __IFRET( scale );
    __IFRET( xYz_rPhiZ );
    __IFRET( uVw_skewPhirMs );

    dt__throw(create(), << str <<  " could not be created");  
  }

  dtTransformer* dtTransformerFactory::create( std::string const str) {
    return create( str.c_str() );
  }
}