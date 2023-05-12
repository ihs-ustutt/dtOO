#ifndef meshQuality_H
#define	meshQuality_H

#include <dtOOTypeDef.h>

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
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        lvH_dtCase const * const dC,      
        lvH_dtPlugin const * const pL
      );    
      virtual void apply(void);
    private:
      dtXmlParser const * _parser;
      dtCase const * _case;
      static bool _registrated;
  };
}

#endif	/* meshQuality_H */

