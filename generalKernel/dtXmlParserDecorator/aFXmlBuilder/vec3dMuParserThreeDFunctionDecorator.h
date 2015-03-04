#ifndef vec3dMuParserThreeDFunctionDecorator_H
#define	vec3dMuParserThreeDFunctionDecorator_H

#include <dtXmlParserDecorator/dtXmlParserFunctionDecorator.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;

  class vec3dMuParserThreeDFunctionDecorator : public dtXmlParserFunctionDecorator {
  public:  
    dt__CLASSNAME(vec3dMuParserThreeDFunctionDecorator);
    vec3dMuParserThreeDFunctionDecorator();
    virtual ~vec3dMuParserThreeDFunctionDecorator();
    virtual void buildPart(
      QDomElement const & toBuildP, 
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP, 
      vectorHandling< analyticFunction * > const * const depSFunP,
      vectorHandling< analyticFunction * > * sFunP
    ) const;
  };
}
#endif	/* vec3dMuParserThreeDFunctionDecorator_H */