#include "boundedVolumeFactory.h"

#include <logMe/logMe.h>
#include "boundedVolume.h"
#include "map3dTo3dCompoundVolume.h"
#include "map3dTo3dTriangulated.h"
#include "readMOABMesh.h"

#define __IFRET(className) \
    if ( strcmp(str, #className) == 0 ) { \
      return new className; \
    }

namespace dtOO {
  boundedVolumeFactory::boundedVolumeFactory() {
  }

  boundedVolumeFactory::~boundedVolumeFactory() {
  }

  boundedVolume * boundedVolumeFactory::create(char const * const str) {
    DTINFOWF(create(), << "creating " << str <<  "...");
    
		__IFRET(map3dTo3dCompoundVolume);
		__IFRET(map3dTo3dTriangulated);
		__IFRET(readMOABMesh)

    dt__THROW(create(), << str <<  " could not be created");  
  }

  boundedVolume * boundedVolumeFactory::create( std::string const str ) {
    return create( str.c_str() );
  }
}