#ifndef uRelInChannel_H
#define	uRelInChannel_H

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
  
  class uRelInChannel : public dtPlugin {
    public:
      dt__class(uRelInChannel, dtPlugin);
      dt__classSelfCreate(uRelInChannel);    
      uRelInChannel();
      virtual ~uRelInChannel();
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
      vectorHandling< analyticGeometry const * > _aG;    
      dtXmlParser const * _parser;
      dtCase const * _case;
      std::vector< int > _nP;
      static bool _registrated;
  };
}

#endif	/* uRelInChannel_H */

