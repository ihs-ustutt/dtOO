#ifndef MAP2DTO3DTRIANGULATED_H
#define	MAP2DTO3DTRIANGULATED_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "gmshBoundedVolume.h"

namespace dtOO {
  class baseContainer;
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class map2dTo3d;
  
  class map2dTo3dTriangulated : public gmshBoundedVolume {
    public:
      dt__class(map2dTo3dTriangulated, boundedVolume);    
      dt__classSelfCreate(map2dTo3dTriangulated);    
      map2dTo3dTriangulated();
      virtual ~map2dTo3dTriangulated();
      virtual void init( 
        ::QDomElement const & element,
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV
      );
    private:
      static bool _registrated;      
  };
}

#endif	/* MAP2DTO3DTRIANGULATED_H */

