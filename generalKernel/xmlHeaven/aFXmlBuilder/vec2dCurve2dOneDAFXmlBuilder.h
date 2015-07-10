#ifndef vec2dCurve2dOneDAFXmlBuilder_H
#define	vec2dCurve2dOneDAFXmlBuilder_H

#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;

  class vec2dCurve2dOneDAFXmlBuilder : public aFXmlBuilder {
  public:
    dt__classOnlyName(vec2dCurve2dOneDAFXmlBuilder);    
    vec2dCurve2dOneDAFXmlBuilder();
    virtual ~vec2dCurve2dOneDAFXmlBuilder();
    virtual void buildPart(
      ::QDomElement const & toBuildP, 
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP, 
      vectorHandling< analyticFunction * > const * const depSFunP,
      vectorHandling< analyticFunction * > * sFunP
    ) const;
  };
}
#endif	/* vec2dCurve2dOneDAFXmlBuilder_H */

