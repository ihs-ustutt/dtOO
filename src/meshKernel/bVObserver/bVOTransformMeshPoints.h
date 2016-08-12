#ifndef bVOTransformMeshPoints_H
#define	bVOTransformMeshPoints_H

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
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< boundedVolume * > const * const bV,
      boundedVolume * attachTo
    );    
    virtual void postUpdate( void );
  private:
    std::vector< dtTransformer const * > _dtT;
    float _relTol;
  };
}
#endif	/* bVOTransformMeshPoints_H */

