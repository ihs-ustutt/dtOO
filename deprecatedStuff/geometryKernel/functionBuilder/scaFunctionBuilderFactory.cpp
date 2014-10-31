#include "scaFunctionBuilderFactory.h"
#include "scaFunctionBuilder.h"
#include "scaMuParserOneDBuilder.h"
#include "scaSISLSplineTwoDBuilder.h"
#include <logMe/logMe.h>

namespace dtOO {
  scaFunctionBuilderFactory::scaFunctionBuilderFactory() {
  }

  scaFunctionBuilderFactory::~scaFunctionBuilderFactory() {
  }

  scaFunctionBuilder* scaFunctionBuilderFactory::create(char const * const str) const {
    DTINFOWF(create(),
            << "creating " << str <<  "...");  
    if ( (strcmp(str, "scaMuParserOneDBuilder") == 0) ||
           (strcmp(str, "scaMuParserOneD") == 0) ) {
      return new scaMuParserOneDBuilder;
    }
    if ( (strcmp(str, "scaSISLSplineTwoDBuilder") == 0) ||
           (strcmp(str, "scaSISLSplineTwoD") == 0) ) {
      return new scaSISLSplineTwoDBuilder;
    }
    dt__THROW(create(),
            << str <<  " could not be created");   
  }
}