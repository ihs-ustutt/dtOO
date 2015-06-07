#include "bVOInterfaceFactory.h"

#include <logMe/logMe.h>
#include <string>
#include "bVOInterfaceFactory.h"
#include "bVOInterface.h"
#include "bVOSetNElements.h"
#include "bVOSetGrading.h"
#include "bVOSetSimpleGrading.h"
#include "bVONameFaces.h"
#include "bVONameRegions.h"
#include "bVOPostBLGmsh.h"
#include "bVOWriteMSH.h"
#include "bVOTransfiniteFaces.h"

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
    dt__info(create(), << str <<  " creating ... ");
    
    __IFRET(bVOSetNElements);
		__IFRET(bVOSetGrading);
		__IFRET(bVOSetSimpleGrading);
		__IFRET(bVONameFaces);
		__IFRET(bVONameRegions);
		__IFRET(bVOPostBLGmsh);
		__IFRET(bVOWriteMSH);
    __IFRET(bVOTransfiniteFaces);
		
    dt__throw(create(), <<  "Could not be created.");
  }
  
  bVOInterface * bVOInterfaceFactory::create(std::string const str) {
    return create(str.c_str());
  }
}