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
        baseContainer * const bC,
        cVPtrVec const * const cV,
        aFPtrVec const * const aF,
        aGPtrVec const * const aG,
        bVPtrVec const * const bV,
        dCPtrVec const * const dC,      
        dPPtrVec const * const pL
      );    
      virtual void apply(void);
    private:
      baseContainer const * _bC;
      cVPtrVec const * _cV;
      aFPtrVec const * _aF;
      aGPtrVec const * _aG;
      std::vector< std::string > _assignRule;
      static bool _registrated;
  };
}
#endif	/* constValueAssingRule_H */

