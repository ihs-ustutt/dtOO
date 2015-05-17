#ifndef vec3dTransVolThreeDAFXmlBuilder_H
#define	vec3dTransVolThreeDAFXmlBuilder_H

#include <dtXmlParserDecorator/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;
  
  class vec3dTransVolThreeDAFXmlBuilder : public aFXmlBuilder {
  public:
    dt__classOnlyName(vec3dTransVolThreeDAFXmlBuilder);    
    vec3dTransVolThreeDAFXmlBuilder();
    virtual ~vec3dTransVolThreeDAFXmlBuilder();
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
#endif	/* vec3dTransVolThreeDAFXmlBuilder_H */

