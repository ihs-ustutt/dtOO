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
    bVONameRegions();
    virtual ~bVONameRegions();
    virtual void init(
      ::QDomElement const & element,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< boundedVolume * > const * const bV,
      boundedVolume * attachTo
    );    
    virtual void preUpdate( void );
    using bVOInterface::postUpdate;
  private:
    std::vector< std::string > _regionLabel;
  };
}
#endif	/* bVONameRegions_H */

