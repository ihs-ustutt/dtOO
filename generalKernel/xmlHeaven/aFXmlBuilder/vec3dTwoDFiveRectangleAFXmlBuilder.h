#ifndef vec3dTwoDFiveRectangleAFXmlBuilder_H
#define	vec3dTwoDFiveRectangleAFXmlBuilder_H

#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class dtCurve;
  class baseContainer;
  
  class vec3dTwoDFiveRectangleAFXmlBuilder : public aFXmlBuilder {
    public:
      dt__classOnlyName(vec3dTwoDFiveRectangleAFXmlBuilder);    
      vec3dTwoDFiveRectangleAFXmlBuilder();
      virtual ~vec3dTwoDFiveRectangleAFXmlBuilder();
      virtual void buildPart(
        QDomElement const & toBuildP, 
        baseContainer * const bC,
        vectorHandling< constValue * > const * const cValP, 
        vectorHandling< analyticFunction * > const * const depSFunP,
        vectorHandling< analyticFunction * > * sFunP) const;
    private:
      static void reorder(vectorHandling< dtCurve const * > & cc);    
  };
}
#endif	/* vec3dTwoDFiveRectangleAFXmlBuilder_H */

