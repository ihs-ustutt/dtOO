#ifndef volVectorOnRotatingLineFieldRange_H
#define	volVectorOnRotatingLineFieldRange_H

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
  
  class volVectorOnRotatingLineFieldRange : public dtPlugin {
  public:
    dt__class(volVectorOnRotatingLineFieldRange, dtPlugin);
    dt__classSelfCreate(volVectorOnRotatingLineFieldRange);    
    volVectorOnRotatingLineFieldRange();
    virtual ~volVectorOnRotatingLineFieldRange();
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
		analyticGeometry const * _aG;    
    dtXmlParser const * _parser;
    dtCase const * _case;
    std::string _field;
    float _min;
    float _max;    
    int _nP;
    bool _noRange;
    dtVector3 _rotAxis;
    dtVector3 _refAxis;
    dtPoint3 _origin;
    float _relBandwith;
  };
}

#endif	/* volVectorOnRotatingLineFieldRange_H */

