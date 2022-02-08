#ifndef map3dTo3dWithInternalGmsh_H
#define	map3dTo3dWithInternalGmsh_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "gmshBoundedVolume.h"

namespace dtOO {
  class baseContainer;
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class map3dTo3d;
  class map2dTo3d;
  
  class map3dTo3dWithInternalGmsh : public gmshBoundedVolume {
    public:
      dt__class(map3dTo3dWithInternalGmsh, boundedVolume);
      dt__classSelfCreate(map3dTo3dWithInternalGmsh);
      map3dTo3dWithInternalGmsh();
      virtual ~map3dTo3dWithInternalGmsh();
      virtual void init( 
        ::QDomElement const & element,
        baseContainer * const bC,
        cVPtrVec const * const cV,
        aFPtrVec const * const aF,
        aGPtrVec const * const aG,
        bVPtrVec const * const bV
      );    
    private:
      dt__pH(map3dTo3d) _m3d;    
      dt__pVH(map2dTo3d) _internal;
  };
}
#endif	/* map3dTo3dWithInternalGmsh_H */

