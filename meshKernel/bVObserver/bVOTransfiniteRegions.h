#ifndef bVOTransfiniteRegions_H
#define	bVOTransfiniteRegions_H

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
    bVOTransfiniteRegions();
    virtual ~bVOTransfiniteRegions();
    virtual void init(
      QDomElement const & element,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< boundedVolume * > const * const bV,
      boundedVolume * attachTo
    );    
    virtual void preUpdate( void );
  private:
    std::vector< std::string > _regionLabel;
  };
}
#endif	/* bVOTransfiniteRegions_H */

