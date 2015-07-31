#ifndef constValueAssingRule_H
#define	constValueAssingRule_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

#include "dtPlugin.h"

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class boundedVolume;
  class baseContainer;
  
  class constValueAssingRule : public dtPlugin {
  public:
    dt__class(constValueAssingRule, dtPlugin);
    constValueAssingRule();
    virtual ~constValueAssingRule();
    virtual void init( 
      ::QDomElement const & element,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< boundedVolume * > const * const bV,
      vectorHandling< dtPlugin * > const * const pL
    );    
    virtual void apply(void);
  private:
    std::vector< constValue * > _cV;
    std::vector< float > _val;
  };
}
#endif	/* constValueAssingRule_H */

