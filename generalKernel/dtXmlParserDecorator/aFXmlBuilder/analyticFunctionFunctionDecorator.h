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
    dt__CLASSNAME(analyticFunctionFunctionDecorator);
    analyticFunctionFunctionDecorator();
    virtual ~analyticFunctionFunctionDecorator();
    virtual void buildPart(
      QDomElement const & toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,  
      vectorHandling< analyticFunction * > const * const depSFunP,
      vectorHandling< analyticFunction * > * sFunP 
    ) const;
  };
}
#endif	/* ANALYTICFUNCTIONFUNCTIONDECORATOR_H */
