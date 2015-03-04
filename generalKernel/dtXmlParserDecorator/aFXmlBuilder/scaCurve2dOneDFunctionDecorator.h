#ifndef scaCurve2dOneDFUNCTIONDECORATOR_H
#define	scaCurve2dOneDFUNCTIONDECORATOR_H

#include <dtXmlParserDecorator/dtXmlParserFunctionDecorator.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;

  class scaCurve2dOneDFunctionDecorator : public dtXmlParserFunctionDecorator {
  public:
    dt__CLASSNAME(scaCurve2dOneDFunctionDecorator);    
    scaCurve2dOneDFunctionDecorator();
    virtual ~scaCurve2dOneDFunctionDecorator();
    virtual void buildPart(
      QDomElement const & toBuildP, 
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP, 
      vectorHandling< analyticFunction * > const * const depSFunP,
      vectorHandling< analyticFunction * > * sFunP
    ) const;
  };
}
#endif	/* scaCurve2dOneDFUNCTIONDECORATOR_H */

