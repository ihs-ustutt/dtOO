#ifndef bVORemoveElements_H
#define	bVORemoveElements_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "bVOInterface.h"
#include <interfaceHeaven/twoDArrayHandling.h>

namespace dtOO {
  class boundedVolume;
  class dtGmshEdge;
  
  class bVORemoveElements : public bVOInterface {
    public:
      dt__class(bVORemoveElements, bVOInterface);
      dt__classSelfCreate(bVORemoveElements);
      bVORemoveElements();
      virtual ~bVORemoveElements();
      virtual void init(
        ::QDomElement const & element,
        baseContainer const * const bC,
        cVPtrVec const * const cV,
        aFPtrVec const * const aF,
        aGPtrVec const * const aG,
        bVPtrVec const * const bV,
        boundedVolume * attachTo
      );    
      virtual void postUpdate( void );
      using bVOInterface::postUpdate;
    private:
      std::vector< std::string > _label;
  };
}
#endif	/* bVORemoveElements_H */

