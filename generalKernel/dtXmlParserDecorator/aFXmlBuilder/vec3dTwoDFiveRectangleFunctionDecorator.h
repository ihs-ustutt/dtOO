#ifndef VEC3DTWODFIVERECTANGLEFUNCTIONDECORATOR_H
#define	VEC3DTWODFIVERECTANGLEFUNCTIONDECORATOR_H

#include <dtXmlParserDecorator/dtXmlParserFunctionDecorator.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class dtCurve;
  class baseContainer;
  
  class vec3dTwoDFiveRectangleFunctionDecorator : public dtXmlParserFunctionDecorator {
    public:
      dt__classOnlyName(vec3dTwoDFiveRectangleFunctionDecorator);    
      vec3dTwoDFiveRectangleFunctionDecorator();
      virtual ~vec3dTwoDFiveRectangleFunctionDecorator();
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
#endif	/* VEC3DTWODFIVERECTANGLEFUNCTIONDECORATOR_H */

