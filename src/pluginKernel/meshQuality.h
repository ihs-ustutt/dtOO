#ifndef meshQuality_H
#define	meshQuality_H

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
  
  class meshQuality : public dtPlugin {
  public:
    dt__class(meshQuality, dtPlugin);
    dt__classSelfCreate(meshQuality);    
    meshQuality();
    virtual ~meshQuality();
    virtual void init( 
      ::QDomElement const & element,
      baseContainer * const bC,
      cVPtrVec const * const cV,
      aFPtrVec const * const aF,
      aGPtrVec const * const aG,
      bVPtrVec const * const bV,
      dCPtrVec const * const dC,      
      dPPtrVec const * const pL
    );    
    virtual void apply(void);
  private:
    dtXmlParser const * _parser;
    dtCase const * _case;
  };
}

#endif	/* meshQuality_H */

