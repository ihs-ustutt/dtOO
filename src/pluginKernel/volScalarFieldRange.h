#ifndef volScalarFieldRange_H
#define	volScalarFieldRange_H

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
  
  class volScalarFieldRange : public dtPlugin {
    public:
      dt__class(volScalarFieldRange, dtPlugin);
      dt__classSelfCreate(volScalarFieldRange);    
      volScalarFieldRange();
      virtual ~volScalarFieldRange();
      virtual void init( 
        ::QDomElement const & element,
        baseContainer const * const bC,
        cVPtrVec const * const cV,
        aFPtrVec const * const aF,
        aGPtrVec const * const aG,
        bVPtrVec const * const bV,
        dCPtrVec const * const dC,      
        dPPtrVec const * const pL
      );    
      virtual void apply(void);
    private:
      dt__pH(scaOneD const) _inRange;    
      dtCase const * _case;
      std::string _field;
  };
}

#endif	/* volScalarFieldRange_H */

