#ifndef bVONameRegions_H
#define	bVONameRegions_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "bVOInterface.h"
#include <interfaceHeaven/twoDArrayHandling.h>

namespace dtOO {
  class boundedVolume;
  class dtGmshEdge;
  
  class bVONameRegions : public bVOInterface {
    public:
      dt__class(bVONameRegions, bVOInterface);
      dt__classSelfCreate(bVONameRegions);
      bVONameRegions();
      virtual ~bVONameRegions();
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
#endif	/* bVONameRegions_H */

