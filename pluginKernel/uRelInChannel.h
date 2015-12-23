#ifndef uRelInChannel_H
#define	uRelInChannel_H

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
  
  class uRelInChannel : public dtPlugin {
  public:
    dt__class(uRelInChannel, dtPlugin);
    uRelInChannel();
    virtual ~uRelInChannel();
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
		vectorHandling< analyticGeometry const * > _aG;    
    dtXmlParser const * _parser;
    dtCase const * _case;
    std::vector< int > _nP;
  };
}

#endif	/* uRelInChannel_H */

