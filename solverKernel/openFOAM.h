#ifndef openFOAM_H
#define	openFOAM_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtCase.h"

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class boundedVolume;
  class baseContainer;
  
  class openFOAM : public dtCase {
  public:
    dt__CLASSSTD(openFOAM, dtCase);
    openFOAM();
    virtual ~openFOAM();
    virtual void init( 
      QDomElement const & element,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< boundedVolume * > const * const bV,
      vectorHandling< dtCase * > const * const dC
    );    
    virtual void apply(void);
  private:
  };
}
#endif	/* openFOAM_H */

