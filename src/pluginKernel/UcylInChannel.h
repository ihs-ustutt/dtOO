#ifndef UcylInChannel_H
#define	UcylInChannel_H

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
  
  class UcylInChannel : public dtPlugin {
  public:
    dt__class(UcylInChannel, dtPlugin);
    dt__classSelfCreate(UcylInChannel);    
    UcylInChannel();
    virtual ~UcylInChannel();
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
		vectorHandling< analyticGeometry const * > _aG;    
    dtXmlParser const * _parser;
    dtCase const * _case;
//    std::string _field;
    std::vector< int > _nP;
    dtVector3 _axis;
    dtPoint3 _origin;
  };
}

#endif	/* UcylInChannel_H */

