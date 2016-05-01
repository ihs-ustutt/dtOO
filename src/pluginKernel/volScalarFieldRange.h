#ifndef volScalarFieldRange_H
#define	volScalarFieldRange_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

#include "dtPlugin.h"

namespace dtOO {
  class dtXmlParser;
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class boundedVolume;
  class baseContainer;
  class dtCase;
  
  class volScalarFieldRange : public dtPlugin {
  public:
    dt__class(volScalarFieldRange, dtPlugin);
    volScalarFieldRange();
    virtual ~volScalarFieldRange();
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
    dtXmlParser const * _parser;
    dtCase const * _case;
    std::string _field;
    float _min;
    float _max;
//    dtVector3 _axis;
//    dtPoint3 _origin;
  };
}

#endif	/* volScalarFieldRange_H */

