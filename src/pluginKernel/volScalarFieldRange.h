#ifndef volScalarFieldRange_H
#define	volScalarFieldRange_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

#include "dtPlugin.h"

namespace dtOO {
//  class dtXmlParser;
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class boundedVolume;
  class baseContainer;
  class dtCase;
  class scaOneD;
  class scaThreeD;
  
  class volScalarFieldRange : public dtPlugin {
    public:
      dt__class(volScalarFieldRange, dtPlugin);
      dt__classSelfCreate(volScalarFieldRange);    
      volScalarFieldRange();
      virtual ~volScalarFieldRange();
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
      dt__pH(scaOneD const) _inRange;    
      dt__pH(scaThreeD const) _inPosition;    
      dtCase const * _case;
      std::string _field;
      static bool _registrated;
  };
}

#endif	/* volScalarFieldRange_H */

