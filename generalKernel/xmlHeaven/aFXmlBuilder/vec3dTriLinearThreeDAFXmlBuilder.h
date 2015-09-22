#ifndef vec3dTriLinearThreeDAFXmlBuilder_H
#define	vec3dTriLinearThreeDAFXmlBuilder_H

#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;
  
  class vec3dTriLinearThreeDAFXmlBuilder : public aFXmlBuilder {
  public:
    dt__classOnlyName(vec3dTriLinearThreeDAFXmlBuilder);    
    vec3dTriLinearThreeDAFXmlBuilder();
    virtual ~vec3dTriLinearThreeDAFXmlBuilder();
    virtual void buildPart(
      ::QDomElement const & toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP, 
      vectorHandling< analyticFunction * > const * const depSFunP,
      vectorHandling< analyticFunction * > * sFunP
    ) const;
  };
}
#endif	/* vec3dTriLinearThreeDAFXmlBuilder_H */

