#ifndef bVORecombineRecursiveRegions_H
#define	bVORecombineRecursiveRegions_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "bVOInterface.h"
#include <vector>

namespace dtOO {
  class boundedVolume;
  class dtGmshEdge;
  
  class bVORecombineRecursiveRegions : public bVOInterface {
    public:
      dt__class(bVORecombineRecursiveRegions, bVOInterface);
      dt__classSelfCreate(bVORecombineRecursiveRegions);
      bVORecombineRecursiveRegions();
      virtual ~bVORecombineRecursiveRegions();
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
#endif	/* bVORecombineRecursiveRegions_H */

