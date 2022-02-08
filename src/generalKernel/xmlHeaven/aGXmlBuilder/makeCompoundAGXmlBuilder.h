#ifndef makeCompoundAGXmlBuilder_H
#define	makeCompoundAGXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class makeCompoundAGXmlBuilder : public aGXmlBuilder {
    public:  
      dt__classOnlyName(makeCompoundAGXmlBuilder);
      dt__classSelfCreate(makeCompoundAGXmlBuilder);
      makeCompoundAGXmlBuilder();
      virtual ~makeCompoundAGXmlBuilder();
      virtual void buildPart(
        ::QDomElement const & toBuild,
        baseContainer * const bC,
        cVPtrVec const * const cV,  
        aFPtrVec const * const aF,  
        aGPtrVec const * const aG,
        aGPtrVec * result 
      ) const; 
    private: 
      static bool _registrated;          
  };
}
#endif	/* makeCompoundAGXmlBuilder_H */

