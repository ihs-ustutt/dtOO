#ifndef bVOTransformMeshPoints_H
#define	bVOTransformMeshPoints_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "bVOInterface.h"
#include <vector>

namespace dtOO {
  class boundedVolume;
  class dtTransformer;
  
  class bVOTransformMeshPoints : public bVOInterface {
    public:
      dt__class(bVOTransformMeshPoints, bVOInterface);
      dt__classSelfCreate(bVOTransformMeshPoints);
      bVOTransformMeshPoints();
      virtual ~bVOTransformMeshPoints();
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
    private:
      std::vector< dtTransformer const * > _dtT;
      dtReal _relTol;
      dtReal _absTol;
      bool _copy;
  };
}
#endif	/* bVOTransformMeshPoints_H */

