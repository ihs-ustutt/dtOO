#ifndef ANALYTICFUNCTIONFUNCTIONDECORATOR_H
#define	ANALYTICFUNCTIONFUNCTIONDECORATOR_H

#include <dtXmlParserDecorator/dtXmlParserFunctionDecorator.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class scaFunction;
  class constValue;
  class baseContainer;

  class analyticFunctionFunctionDecorator : public dtXmlParserFunctionDecorator {
  public:  
    dt__classOnlyName(analyticFunctionFunctionDecorator);
    analyticFunctionFunctionDecorator();
    virtual ~analyticFunctionFunctionDecorator();
    virtual void buildPart(
      QDomElement const & toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cV,  
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticFunction * > * result 
    ) const;
  };
}
#endif	/* ANALYTICFUNCTIONFUNCTIONDECORATOR_H */

