#ifndef volVectorFieldRange_H
#define	volVectorFieldRange_H

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
  class scaThreeD;
  
  class volVectorFieldRange : public dtPlugin {
    public:
      dt__class(volVectorFieldRange, dtPlugin);
      dt__classSelfCreate(volVectorFieldRange);    
      volVectorFieldRange();
      virtual ~volVectorFieldRange();
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
      dt__pH(scaThreeD const) _inRange;    
      dtCase const * _case;
      std::string _field;
  };
}

#endif	/* volVectorFieldRange_H */
