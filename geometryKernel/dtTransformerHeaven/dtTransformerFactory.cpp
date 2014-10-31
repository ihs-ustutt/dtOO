#include "dtTransformerFactory.h"
#include <logMe/logMe.h>
#include "conformalMapping.h"
#include "dtTransformer.h"
#include "doNothing.h"
#include "thicknessIncreasing.h"
#include "biThicknessIncreasing.h"
#include "offset.h"
#include "rotate.h"
#include "conformalMapping.h"
#include "pickRange.h"
#include "conformalMappingStartFromPoint.h"
#include "makePolynomial.h"
#include "predefinedExtension.h"
#include "closeGaps.h"

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

  dtTransformer* dtTransformerFactory::create(char const * const str) const {
    DTINFOWF(create(), << "creating " << str <<  "...");
    
    __IFRET( doNothing );
    __IFRET( conformalMapping );
    __IFRET( thicknessIncreasing );
		__IFRET( biThicknessIncreasing );
    __IFRET( offset );
    __IFRET( rotate );
    __IFRET( pickRange );
    __IFRET( conformalMappingStartFromPoint );
    __IFRET( makePolynomial );
    __IFRET( predefinedExtension );
    __IFRET( closeGaps );

    dt__THROW(create(), << str <<  " could not be created");  
  }

  dtTransformer* dtTransformerFactory::create(string const str) const {
    return create( str.c_str() );
  }
}