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
#include "bVOWriteMSH.h"
#include "bVOTransfiniteFaces.h"
#include "bVOTransfiniteRegions.h"
#include "bVOOrientCellVolumes.h"
#include "bVORecombineRecursiveRegions.h"
#include "bVOMeshRule.h"
#include "bVOPatchRule.h"
#include "bVORecombine.h"
#include "bVOWriteSTL.h"

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
		__IFRET(bVOWriteMSH);
    __IFRET(bVOTransfiniteFaces);
    __IFRET(bVOTransfiniteRegions);
    __IFRET(bVOOrientCellVolumes);
    __IFRET(bVORecombineRecursiveRegions);
    __IFRET(bVOMeshRule);
    __IFRET(bVOPatchRule);
    __IFRET(bVORecombine);
    __IFRET(bVOWriteSTL);
		
    dt__throw(create(), <<  "Could not be created.");
  }
  
  bVOInterface * bVOInterfaceFactory::create(std::string const str) {
    return create(str.c_str());
  }
}