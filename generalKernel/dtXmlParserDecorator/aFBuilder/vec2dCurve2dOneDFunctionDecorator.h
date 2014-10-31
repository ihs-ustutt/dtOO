#ifndef VEC2DCURVE2DONEDFUNCTIONDECORATOR_H
#define	VEC2DCURVE2DONEDFUNCTIONDECORATOR_H

#include <dtXmlParserDecorator/dtXmlParserFunctionDecorator.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;

  class vec2dCurve2dOneDFunctionDecorator : public dtXmlParserFunctionDecorator {
  public:
    dt__CLASSNAME(vec2dCurve2dOneDFunctionDecorator);    
    vec2dCurve2dOneDFunctionDecorator();
    virtual ~vec2dCurve2dOneDFunctionDecorator();
    virtual void buildPart(QDomElement const & toBuildP, 
                                          vectorHandling< constValue * > const * const cValP, 
                                          vectorHandling< analyticFunction * > const * const depSFunP,
                                          vectorHandling< analyticFunction * > * sFunP) const;
  };
}
#endif	/* VEC2DCURVE2DONEDFUNCTIONDECORATOR_H */

