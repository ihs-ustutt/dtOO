#ifndef analyticFunctionCombinationAFXmlBuilder_H
#define	analyticFunctionCombinationAFXmlBuilder_H

#include <dtOOTypeDef.h>

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
        lvH_constValue const * const cV, 
        lvH_analyticFunction const * const aF,
        lvH_analyticFunction * ret
      ) const;
    private: 
      static bool _registrated;    
  };
}
#endif	/* analyticFunctionCombinationAFXmlBuilder_H */