#ifndef ANALYTICFUNCTIONAFXmlBuilder_H
#define	ANALYTICFUNCTIONAFXmlBuilder_H

#include <dtXmlParserDecorator/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class scaFunction;
  class constValue;
  class baseContainer;

  class analyticFunctionAFXmlBuilder : public aFXmlBuilder {
  public:  
    dt__classOnlyName(analyticFunctionAFXmlBuilder);
    analyticFunctionAFXmlBuilder();
    virtual ~analyticFunctionAFXmlBuilder();
    virtual void buildPart(
      QDomElement const & toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cV,  
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticFunction * > * result 
    ) const;
  };
}
#endif	/* ANALYTICFUNCTIONAFXmlBuilder_H */

