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
    map3dTo3dGmsh();
    virtual ~map3dTo3dGmsh();
    virtual void init( 
      ::QDomElement const & element,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< boundedVolume * > const * const bV
    );    
  };
}
#endif	/* map3dTo3dGmsh_H */

