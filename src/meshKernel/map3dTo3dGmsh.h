#ifndef map3dTo3dGmsh_H
#define	map3dTo3dGmsh_H

#include <logMe/dtMacros.h>
#include "gmshBoundedVolume.h"

namespace dtOO {
  class baseContainer;
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class dtGmshModel;
  class map3dTo3d;
   
  class map3dTo3dGmsh : public gmshBoundedVolume {
  public:
    dt__class(map3dTo3dGmsh, boundedVolume);
    dt__classSelfCreate(map3dTo3dGmsh);
    map3dTo3dGmsh();
    virtual ~map3dTo3dGmsh();
    virtual void init( 
      ::QDomElement const & element,
      baseContainer const * const bC,
      cVPtrVec const * const cV,
      aFPtrVec const * const aF,
      aGPtrVec const * const aG,
      bVPtrVec const * const bV
    );    
  };
}
#endif	/* map3dTo3dGmsh_H */

