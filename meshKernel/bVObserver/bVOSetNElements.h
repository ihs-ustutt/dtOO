#ifndef bVOSetNElements_H
#define	bVOSetNElements_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "bVOInterface.h"
#include <interfaceHeaven/twoDArrayHandling.h>

namespace dtOO {
  class boundedVolume;
  class dtGmshEdge;
  
  class bVOSetNElements : public bVOInterface {
  public:
    dt__CLASSNAME(bVOSetNElements);
    bVOSetNElements();
    virtual ~bVOSetNElements();
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
    int _nU;
    int _nV;
    int _nW;
  };
}
#endif	/* bVOSetNElements_H */
