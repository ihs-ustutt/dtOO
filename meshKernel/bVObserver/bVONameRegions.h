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
    dt__CLASSNAME(bVONameRegions);
    bVONameRegions();
    virtual ~bVONameRegions();
    virtual void init(
      QDomElement const & element,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< boundedVolume * > const * const bV,
      boundedVolume * attachTo
    );    
    virtual void update( void );
  private:
    std::vector< std::string > _regionLabel;
  };
}
#endif	/* bVONameRegions_H */
