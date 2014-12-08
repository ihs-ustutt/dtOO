#ifndef WRITESTEP_H
#define	WRITESTEP_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

#include "dtPlugin.h"

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class boundedVolume;
  
  class writeStep : public dtPlugin {
  public:
    dt__CLASSSTD(writeStep, dtPlugin);
    writeStep();
    virtual ~writeStep();
    virtual void init( 
      QDomElement const & element,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< boundedVolume * > const * const bV,
      vectorHandling< dtPlugin * > const * const pL
    );    
    virtual void apply(void);
  private:
    vectorHandling< analyticGeometry * > _aG;
  };
}
#endif	/* WRITESTEP_H */

