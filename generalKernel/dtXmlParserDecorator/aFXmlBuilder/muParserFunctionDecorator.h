#ifndef muParserFunctionDecorator_H
#define	muParserFunctionDecorator_H

#include <dtXmlParserDecorator/dtXmlParserFunctionDecorator.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;

  class muParserFunctionDecorator : public dtXmlParserFunctionDecorator {
  public:  
    dt__CLASSNAME(muParserFunctionDecorator);
    muParserFunctionDecorator();
    virtual ~muParserFunctionDecorator();
    virtual void buildPart(
      QDomElement const & toBuildP, 
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP, 
      vectorHandling< analyticFunction * > const * const depSFunP,
      vectorHandling< analyticFunction * > * sFunP
    ) const;
  };
}
#endif	/* muParserFunctionDecorator_H */