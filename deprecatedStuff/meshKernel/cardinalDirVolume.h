#ifndef CARDINALDIRVOLUME_H
#define	CARDINALDIRVOLUME_H

#include <dtLinearAlgebra.h>
#include "boundedVolume.h"
#include <interfaceHeaven/coDoSetHandling.h>
#include "cardinalDirVolumeDefines.h"

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  
  class cardinalDirVolume : public boundedVolume {
  public:
    dt__classOnlyName(cardinalDirVolume);
    cardinalDirVolume();
    virtual ~cardinalDirVolume();
    virtual void init(
      QDomElement const & element,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP,
      vectorHandling< analyticGeometry * > const * const depAGeoP,
      vectorHandling< boundedVolume * > const * const depBVolP
    );
    bool hasInternal( void ) const;
    virtual covise::coDoSet * toCoDoSet(char const * const str) const;    
  private:
  };
}
#endif	/* CARDINALDIRVOLUME_H */

