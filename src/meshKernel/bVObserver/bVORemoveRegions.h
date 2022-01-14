#ifndef bVORemoveRegions_H
#define	bVORemoveRegions_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "bVOInterface.h"
#include <interfaceHeaven/twoDArrayHandling.h>

namespace dtOO {
  class boundedVolume;
  class dtGmshEdge;
  
  class bVORemoveRegions : public bVOInterface {
    public:
      dt__class(bVORemoveRegions, bVOInterface);
      dt__classSelfCreate(bVORemoveRegions);
      bVORemoveRegions();
      virtual ~bVORemoveRegions();
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
      using bVOInterface::postUpdate;
    private:
      std::vector< std::string > _regionLabel;
  };
}
#endif	/* bVORemoveRegions_H */

