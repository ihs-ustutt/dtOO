#include "boundedVolumeFactory.h"

#include <logMe/logMe.h>
#include "boundedVolume.h"
#include "map3dTo3dCompoundVolume.h"

#define __IFRET(className) \
    if ( strcmp(str, #className) == 0 ) { \
      return new className; \
    }
#define __IFRETCUSTOM(className, retClass) \
  if ( strcmp(str, #className) == 0 ) { \
    return new retClass; \
  }

namespace dtOO {
  boundedVolumeFactory::boundedVolumeFactory() {
  }

  boundedVolumeFactory::~boundedVolumeFactory() {
  }

  boundedVolume * boundedVolumeFactory::create(char const * const str) {
    DTINFOWF(create(), << "creating " << str <<  "...");
    
		__IFRET(map3dTo3dCompoundVolume);

    dt__THROW(create(), << str <<  " could not be created");  
  }

  boundedVolume * boundedVolumeFactory::create( std::string const str ) {
    return create( str.c_str() );
  }
}