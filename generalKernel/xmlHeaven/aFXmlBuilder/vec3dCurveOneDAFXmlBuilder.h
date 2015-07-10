#ifndef vec3dCurveOneDAFXmlBuilder_H
#define	vec3dCurveOneDAFXmlBuilder_H

#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;
  
  class vec3dCurveOneDAFXmlBuilder : public aFXmlBuilder {
  public:
    dt__classOnlyName(vec3dCurveOneDAFXmlBuilder);    
    vec3dCurveOneDAFXmlBuilder();
    virtual ~vec3dCurveOneDAFXmlBuilder();
    virtual void buildPart(
      ::QDomElement const & toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP, 
      vectorHandling< analyticFunction * > const * const depSFunP,
      vectorHandling< analyticFunction * > * sFunP
    ) const;
  };
}
#endif	/* vec3dCurveOneDAFXmlBuilder_H */

