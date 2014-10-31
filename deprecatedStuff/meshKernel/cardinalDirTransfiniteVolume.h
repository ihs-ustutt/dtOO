#ifndef cardinalDirTransfiniteVolume_H
#define	cardinalDirTransfiniteVolume_H

#include <dtLinearAlgebra.h>
#include "boundedVolume.h"
#include <interfaceHeaven/coDoSetHandling.h>
#include "cardinalDirVolumeDefines.h"

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  
  class cardinalDirTransfiniteVolume : public boundedVolume {
  public:
    dt__CLASSNAME(cardinalDirTransfiniteVolume);
    cardinalDirTransfiniteVolume();
    virtual ~cardinalDirTransfiniteVolume();
    virtual void init(
      QDomElement const & element,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP,
      vectorHandling< analyticGeometry * > const * const depAGeoP,
      vectorHandling< boundedVolume * > const * const depBVolP
    );
    bool hasInternal( void ) const; 
  private:
  };
}
#endif	/* cardinalDirTransfiniteVolume_H */

