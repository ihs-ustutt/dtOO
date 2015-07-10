#ifndef bVORecombineRecursiveRegions_H
#define	bVORecombineRecursiveRegions_H

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
    bVORecombineRecursiveRegions();
    virtual ~bVORecombineRecursiveRegions();
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
  private:
    std::vector< std::string > _regionLabel;
  };
}
#endif	/* bVORecombineRecursiveRegions_H */

