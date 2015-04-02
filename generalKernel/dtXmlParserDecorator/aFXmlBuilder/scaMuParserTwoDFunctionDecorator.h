#ifndef scaMuParserTwoDFunctionDecorator_H
#define	scaMuParserTwoDFunctionDecorator_H

#include <dtXmlParserDecorator/dtXmlParserFunctionDecorator.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;

  class scaMuParserTwoDFunctionDecorator : public dtXmlParserFunctionDecorator {
  public:  
    dt__classOnlyName(scaMuParserTwoDFunctionDecorator);
    scaMuParserTwoDFunctionDecorator();
    virtual ~scaMuParserTwoDFunctionDecorator();
    virtual void buildPart(
      QDomElement const & toBuildP, 
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP, 
      vectorHandling< analyticFunction * > const * const depSFunP,
      vectorHandling< analyticFunction * > * sFunP
    ) const;
  };
}
#endif	/* scaMuParserTwoDFunctionDecorator_H */