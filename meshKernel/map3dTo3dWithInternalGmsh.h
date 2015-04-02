#ifndef map3dTo3dWithInternalGmsh_H
#define	map3dTo3dWithInternalGmsh_H

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
    map3dTo3dWithInternalGmsh();
    virtual ~map3dTo3dWithInternalGmsh();
    virtual void init( 
      QDomElement const & element,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< boundedVolume * > const * const bV
    );    
    virtual void makeGrid(void);
    virtual void makePreGrid(void);
  private:
    dt__pH(map3dTo3d) _m3d;    
    dt__pVH(map2dTo3d) _internal;
    float _bladeNormalWidth;
    float _bladeTangentialWidth;  
  };
}
#endif	/* map3dTo3dWithInternalGmsh_H */

