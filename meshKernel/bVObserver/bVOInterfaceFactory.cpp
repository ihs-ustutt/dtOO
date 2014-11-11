#include "bVOInterfaceFactory.h"

#include <logMe/logMe.h>
#include <string>
#include "bVOInterfaceFactory.h"
#include "bVOInterface.h"
#include "bVOSplitEdge.h"

#define __IFRET(className) \
	if ( strcmp(str, #className) == 0 ) { \
		return new className; \
	}
namespace dtOO {
  bVOInterfaceFactory::bVOInterfaceFactory() {
  }

  bVOInterfaceFactory::~bVOInterfaceFactory() {
  }

  bVOInterface * bVOInterfaceFactory::create(char const * const str) {
    DTINFOWF(create(), << str <<  " creating ... ");
    
    __IFRET(bVOSplitEdge);
		
    dt__THROW(create(), <<  "Could not be created.");
  }
  
  bVOInterface * bVOInterfaceFactory::create(std::string const str) {
    return create(str.c_str());
  }
}