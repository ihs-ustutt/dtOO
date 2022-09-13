#ifndef constValueAssingRule_H
#define	constValueAssingRule_H

#include <dtOOTypeDef.h>

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
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        lvH_dtCase const * const dC,      
        lvH_dtPlugin const * const pL
      );    
      virtual void apply(void);
    private:
      baseContainer const * _bC;
      lvH_constValue const * _cV;
      lvH_analyticFunction const * _aF;
      lvH_analyticGeometry const * _aG;
      std::vector< std::string > _assignRule;
      static bool _registrated;
  };
}
#endif	/* constValueAssingRule_H */

