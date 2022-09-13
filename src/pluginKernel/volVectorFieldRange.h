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
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        lvH_dtCase const * const dC,      
        lvH_dtPlugin const * const pL
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

