#ifndef bVOSplitEdge_H
#define	bVOSplitEdge_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "bVOInterface.h"
#include <interfaceHeaven/twoDArrayHandling.h>

namespace dtOO {
  class boundedVolume;
  class dtGmshEdge;
  
  class bVOSplitEdge : public bVOInterface {
  public:
    dt__CLASSNAME(bVOSplitEdge);
    bVOSplitEdge();
    virtual ~bVOSplitEdge();
    virtual void init(
      QDomElement const & element,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP,
      vectorHandling< analyticGeometry * > const * const depAGeoP,
      vectorHandling< boundedVolume * > const * const depBVolP,
      boundedVolume * bV
    );    
    virtual void update( void );
  private:
    std::vector< int > _edge;
    dtVector3 _scale;
  };
}
#endif	/* bVOSplitEdge_H */

