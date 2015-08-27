#include "dtCaseFactory.h"

#include <logMe/logMe.h>
#include "dtCase.h"

#include "OpenFOAMCase/OpenFOAMCase.h"

namespace dtOO {
  dtCaseFactory::dtCaseFactory() {
  }

  dtCaseFactory::~dtCaseFactory() {
  }

//  dtCase * dtCaseFactory::create(char const * const str) {
//    dt__info(create(), << "creating " << str <<  "...");
//    
//		if ()
//
//    dt__throw(create(), << str <<  " could not be created");  
//  }

  dtCase * dtCaseFactory::create( std::string const str ) {
    dt__info(create(), << "creating " << str <<  "...");    
    
    if (str == "OpenFOAMCase") return new OpenFOAMCase();

    dt__throw(create(), << str <<  " could not be created");      
  }
}