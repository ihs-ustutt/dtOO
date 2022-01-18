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
        cVPtrVec const * const cV,
        aFPtrVec const * const aF,
        aGPtrVec const * const aG,
        bVPtrVec const * const bV,
        boundedVolume * attachTo
      );    
      virtual void preUpdate( void );
    private:
      std::vector< std::string > _regionLabel;
      std::vector< dtInt > _nE;       
  };
}
#endif	/* bVOTransfiniteRegions_H */

