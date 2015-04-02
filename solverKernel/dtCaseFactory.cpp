#include "dtCaseFactory.h"

#include <logMe/logMe.h>
#include "dtCase.h"

#include "openFOAM.h"

#define __IFRET(className) \
    if ( strcmp(str, #className) == 0 ) { \
      return new className; \
    }

namespace dtOO {
  dtCaseFactory::dtCaseFactory() {
  }

  dtCaseFactory::~dtCaseFactory() {
  }

  dtCase * dtCaseFactory::create(char const * const str) {
    dt__info(create(), << "creating " << str <<  "...");
    
		__IFRET(openFOAM);

    dt__throw(create(), << str <<  " could not be created");  
  }

  dtCase * dtCaseFactory::create( std::string const str ) {
    return create( str.c_str() );
  }
}