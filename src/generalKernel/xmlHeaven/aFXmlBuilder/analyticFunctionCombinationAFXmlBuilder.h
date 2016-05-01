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
    analyticFunctionCombinationAFXmlBuilder();
    virtual ~analyticFunctionCombinationAFXmlBuilder();
    virtual void buildPart(
      ::QDomElement const & toBuild, 
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cV, 
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticFunction * > * ret
    ) const;
  };
}
#endif	/* analyticFunctionCombinationAFXmlBuilder_H */