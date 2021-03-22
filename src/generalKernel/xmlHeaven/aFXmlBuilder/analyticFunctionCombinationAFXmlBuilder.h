#ifndef analyticFunctionCombinationAFXmlBuilder_H
#define	analyticFunctionCombinationAFXmlBuilder_H

#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;

  class analyticFunctionCombinationAFXmlBuilder : public aFXmlBuilder {
    public:  
      dt__classOnlyName(analyticFunctionCombinationAFXmlBuilder);
      dt__classSelfCreate(analyticFunctionCombinationAFXmlBuilder);
      analyticFunctionCombinationAFXmlBuilder();
      virtual ~analyticFunctionCombinationAFXmlBuilder();
      virtual void buildPart(
        ::QDomElement const & toBuild, 
        baseContainer * const bC,
        cVPtrVec const * const cV, 
        aFPtrVec const * const aF,
        aFPtrVec * ret
      ) const;
    private: 
      static bool _registrated;    
  };
}
#endif	/* analyticFunctionCombinationAFXmlBuilder_H */