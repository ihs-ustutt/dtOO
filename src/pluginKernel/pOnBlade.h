#ifndef pOnBlade_H
#define	pOnBlade_H

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
  class map1dTo3d;
  class map2dTo3d;
  
  class pOnBlade : public dtPlugin {
  public:
    dt__class(pOnBlade, dtPlugin);
    pOnBlade();
    virtual ~pOnBlade();
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
		map2dTo3d const * _blade;    
    map1dTo3d const * _stagnationLine; 
    dtXmlParser const * _parser;
    dtCase const * _case;
    std::vector< int > _nP;
  };
}

#endif	/* pOnBlade_H */

