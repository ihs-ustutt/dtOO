#ifndef volScalarPatchFieldRange_H
#define	volScalarPatchFieldRange_H

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
  
  class volScalarPatchFieldRange : public dtPlugin {
  public:
    dt__class(volScalarPatchFieldRange, dtPlugin);
    volScalarPatchFieldRange();
    virtual ~volScalarPatchFieldRange();
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
    std::string _patchName;
    bool _noRange;    
  };
}

#endif	/* volScalarPatchFieldRange_H */

