#ifndef bVOTransfiniteRegions_H
#define	bVOTransfiniteRegions_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "bVOInterface.h"
#include <vector>

namespace dtOO {
  class boundedVolume;
  class dtGmshEdge;
  
  class bVOTransfiniteRegions : public bVOInterface {
    public:
      dt__class(bVOTransfiniteRegions, bVOInterface);
      dt__classSelfCreate(bVOTransfiniteRegions);
      bVOTransfiniteRegions();
      virtual ~bVOTransfiniteRegions();
      virtual void init(
        ::QDomElement const & element,
        baseContainer const * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        boundedVolume * attachTo
      );    
      virtual void preUpdate( void );
    private:
      static bool _registrated;
  };
}
#endif	/* bVOTransfiniteRegions_H */

