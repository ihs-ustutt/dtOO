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
    dt__classSelfCreate(pOnBlade);    
    pOnBlade();
    virtual ~pOnBlade();
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
		map2dTo3d const * _blade;    
    map1dTo3d const * _stagnationLine; 
    dtXmlParser const * _parser;
    dtCase const * _case;
    std::vector< int > _nP;
  };
}

#endif	/* pOnBlade_H */

