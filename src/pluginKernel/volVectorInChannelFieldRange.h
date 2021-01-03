#ifndef volVectorInChannelFieldRange_H
#define	volVectorInChannelFieldRange_H

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
  
  class volVectorInChannelFieldRange : public dtPlugin {
  public:
    dt__class(volVectorInChannelFieldRange, dtPlugin);
    dt__classSelfCreate(volVectorInChannelFieldRange);    
    volVectorInChannelFieldRange();
    virtual ~volVectorInChannelFieldRange();
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
		analyticGeometry const * _aG;    
    dtXmlParser const * _parser;
    dtCase const * _case;
    std::string _field;
    float _min;
    float _max;    
    std::vector< int > _nP;
    bool _noRange;
    static bool _registrated;
  };
}

#endif	/* volVectorInChannelFieldRange_H */

