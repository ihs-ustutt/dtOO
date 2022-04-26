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
        cVPtrVec const * const cV,
        aFPtrVec const * const aF,
        aGPtrVec const * const aG,
        bVPtrVec const * const bV,
        boundedVolume * attachTo
      );    
      virtual void preUpdate( void );
    private:
      std::vector< std::string > _regionLabel;
      static bool _registrated;      
  };
}
#endif	/* bVORecombineRecursiveRegions_H */

