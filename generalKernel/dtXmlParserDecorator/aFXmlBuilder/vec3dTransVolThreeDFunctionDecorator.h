#ifndef VEC3DTRANSVOLTHREEDFUNCTIONDECORATOR_H
#define	VEC3DTRANSVOLTHREEDFUNCTIONDECORATOR_H

#include <dtXmlParserDecorator/dtXmlParserFunctionDecorator.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;
  
  class vec3dTransVolThreeDFunctionDecorator : public dtXmlParserFunctionDecorator {
  public:
    dt__CLASSNAME(vec3dTransVolThreeDFunctionDecorator);    
    vec3dTransVolThreeDFunctionDecorator();
    virtual ~vec3dTransVolThreeDFunctionDecorator();
    virtual void buildPart(
      QDomElement const & toBuildP, 
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP, 
      vectorHandling< analyticFunction * > const * const depSFunP,
      vectorHandling< analyticFunction * > * sFunP
    ) const;
    virtual void buildPartCompound(
      QDomElement const & toBuildP, 
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP, 
      vectorHandling< analyticFunction * > const * const depSFunP,
      vectorHandling< analyticFunction * > * sFunP
    ) const;    
  };
}
#endif	/* VEC3DTRANSVOLTHREEDFUNCTIONDECORATOR_H */

