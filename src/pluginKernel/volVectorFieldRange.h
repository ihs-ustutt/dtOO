#ifndef volVectorFieldRange_H
#define	volVectorFieldRange_H

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
  class scaThreeD;
  
  class volVectorFieldRange : public dtPlugin {
    public:
      dt__class(volVectorFieldRange, dtPlugin);
      dt__classSelfCreate(volVectorFieldRange);    
      volVectorFieldRange();
      virtual ~volVectorFieldRange();
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
      dt__pH(scaThreeD const) _inRange;
      dt__pH(scaThreeD const) _inPosition;
      dtCase const * _case;
      std::string _field;
      static bool _registrated;
  };
}

#endif	/* volVectorFieldRange_H */

