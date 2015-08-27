#include "constValueFactory.h"

#include "constValue.h"
#include <logMe/logMe.h>
#include "sliderFloatParam.h"
#include "intParam.h"
#include <string.h>

namespace dtOO {
  constValueFactory::constValueFactory() {
  }

  constValueFactory::~constValueFactory() {
  }

//  constValue * constValueFactory::create(char const * const str) {
//
//    dt__info(create(), << str <<  " creating ... ");
//
//    if ( strcmp(str, "sliderFloatParam") == 0 ) return new sliderFloatParam;  
//    if ( strcmp(str, "intParam") == 0 ) return new intParam;
//    
//    dt__throw(create(), <<  "Could not be created.");
//  }

  constValue* constValueFactory::create(std::string const str) {
    dt__info(create(), << str <<  " creating ... ");

    if (str == "sliderFloatParam") return new sliderFloatParam;  
    if (str == "intParam") return new intParam;
    
    dt__throw(create(), <<  "Could not be created.");
  }
}