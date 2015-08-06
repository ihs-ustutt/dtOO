#include "boundedVolumeFactory.h"

#include <logMe/logMe.h>
#include "boundedVolume.h"
#include "map2dTo3dTriangulated.h"
#include "map3dTo3dWithInternalTurboGrid.h"
#include "vec3dInMap3dTo3dWithMeshedSurface.h"
#include "readMOABMesh.h"
#include "map3dTo3dWithInternalGmsh.h"
#include "map3dTo3dWithInternalBlockGmsh.h"
#include "map3dTo3dGmsh.h"

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
    dt__info(create(), << "creating " << str <<  "...");
    
		__IFRET(map2dTo3dTriangulated);
		__IFRET(map3dTo3dWithInternalTurboGrid);
		__IFRET(vec3dInMap3dTo3dWithMeshedSurface);
		__IFRET(readMOABMesh);
		__IFRET(map3dTo3dWithInternalGmsh);
    __IFRET(map3dTo3dWithInternalBlockGmsh);
    __IFRET(map3dTo3dGmsh);

    dt__throw(create(), << str <<  " could not be created");  
  }

  boundedVolume * boundedVolumeFactory::create( std::string const str ) {
    return create( str.c_str() );
  }
}