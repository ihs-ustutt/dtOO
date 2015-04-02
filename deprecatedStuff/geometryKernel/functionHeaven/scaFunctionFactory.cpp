#include "scaFunctionFactory.h"
#include <logMe/logMe.h>
#include "scaFunction.h"
#include "scaMuParserOneD.h"
#include "scaCurve2dOneD.h"

#define __IFRET(className) \
    if ( strcmp(str, #className) == 0 ) { \
      return new className; \
    }

namespace dtOO {
  scaFunctionFactory::scaFunctionFactory() {
  }

  scaFunctionFactory::~scaFunctionFactory() {
  }

  scaFunction* scaFunctionFactory::create(char const * const str) const {
    dt__info(create(), << "creating " << str <<  "...");  

    __IFRET(scaMuParserOneD);
		__IFRET(scaCurve2dOneD);
		
    dt__THROW(create(), << str <<  " could not be created"); 
  }

  scaFunction* scaFunctionFactory::create(string const str) const {
    return create( str.c_str() );
  }
}