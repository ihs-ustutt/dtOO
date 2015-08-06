#ifndef MAP3DTO3DBLOCKGMSH_H
#define	MAP3DTO3DBLOCKGMSH_H

#include <logMe/dtMacros.h>
#include "gmshBoundedVolume.h"

namespace dtOO {
  class baseContainer;
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class dtGmshModel;
  class map3dTo3d;
   
  class map3dTo3dBlockGmsh : public gmshBoundedVolume {
  public:
    dt__class(map3dTo3dBlockGmsh, boundedVolume);
    map3dTo3dBlockGmsh();
    virtual ~map3dTo3dBlockGmsh();
    virtual void init( 
      ::QDomElement const & element,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< boundedVolume * > const * const bV
    );    
  private:
    dt__pH(map3dTo3d) _m3d;    
  };
}
#endif	/* MAP3DTO3DBLOCKGMSH_H */

