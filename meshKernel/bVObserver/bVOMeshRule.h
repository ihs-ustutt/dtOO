#ifndef bVOMeshRule_H
#define	bVOMeshRule_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "bVOInterface.h"
#include <interfaceHeaven/twoDArrayHandling.h>

namespace dtOO {
  class boundedVolume;
  class dtGmshEdge;
  
  class bVOMeshRule : public bVOInterface {
  public:
    dt__class(bVOMeshRule, bVOInterface);
    bVOMeshRule();
    virtual ~bVOMeshRule();
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
  };
}
#endif	/* bVOMeshRule_H */

