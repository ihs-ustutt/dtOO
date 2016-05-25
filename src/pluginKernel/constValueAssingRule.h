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
  class dtCase;
  
  class constValueAssingRule : public dtPlugin {
  public:
    dt__class(constValueAssingRule, dtPlugin);
    dt__classSelfCreate(constValueAssingRule);     
    constValueAssingRule();
    virtual ~constValueAssingRule();
    virtual void init( 
      ::QDomElement const & element,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< boundedVolume * > const * const bV,
      vectorHandling< dtCase * > const * const dC,      
      vectorHandling< dtPlugin * > const * const pL
    );    
    virtual void apply(void);
  private:
		baseContainer const * _bC;
		vectorHandling< constValue * > const * _cV;
		vectorHandling< analyticFunction * > const * _aF;
		vectorHandling< analyticGeometry * > const * _aG;
    std::vector< std::string > _assignRule;
  };
}
#endif	/* constValueAssingRule_H */

