#ifndef map3dTo3dGmsh_H
#define	map3dTo3dGmsh_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "gmshBoundedVolume.h"

namespace dtOO {
  class baseContainer;
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class dtGmshModel;
   
  class map3dTo3dGmsh : public gmshBoundedVolume {
    public:
      dt__class(map3dTo3dGmsh, boundedVolume);
      dt__classSelfCreate(map3dTo3dGmsh);
      map3dTo3dGmsh();
      virtual ~map3dTo3dGmsh();
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
#endif	/* map3dTo3dGmsh_H */

