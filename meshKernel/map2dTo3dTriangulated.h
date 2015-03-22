#ifndef MAP2DTO3DTRIANGULATED_H
#define	MAP2DTO3DTRIANGULATED_H

#include <logMe/dtMacros.h>
#include "gmshBoundedVolume.h"
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class baseContainer;
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class map2dTo3d;
  
  class map2dTo3dTriangulated : public gmshBoundedVolume {
  public:
    dt__CLASSSTD(map2dTo3dTriangulated, boundedVolume);    
    map2dTo3dTriangulated();
    virtual ~map2dTo3dTriangulated();
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
  };
}

#endif	/* MAP2DTO3DTRIANGULATED_H */

