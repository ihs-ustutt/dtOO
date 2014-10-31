#ifndef VEC3DCURVEONEDFUNCTIONDECORATOR_H
#define	VEC3DCURVEONEDFUNCTIONDECORATOR_H

#include <dtXmlParserDecorator/dtXmlParserFunctionDecorator.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;

  class vec3dCurveOneDFunctionDecorator : public dtXmlParserFunctionDecorator {
  public:
    dt__CLASSNAME(vec3dCurveOneDFunctionDecorator);    
    vec3dCurveOneDFunctionDecorator();
    virtual ~vec3dCurveOneDFunctionDecorator();
    virtual void buildPart(QDomElement const & toBuildP, 
                                          vectorHandling< constValue * > const * const cValP, 
                                          vectorHandling< analyticFunction * > const * const depSFunP,
                                          vectorHandling< analyticFunction * > * sFunP) const;
  };
}
#endif	/* VEC3DCURVEONEDFUNCTIONDECORATOR_H */

