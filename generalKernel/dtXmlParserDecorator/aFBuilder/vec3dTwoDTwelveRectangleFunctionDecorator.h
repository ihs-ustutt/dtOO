#ifndef VEC3DTWODTWELVERECTANGLEFUNCTIONDECORATOR_H
#define	VEC3DTWODTWELVERECTANGLEFUNCTIONDECORATOR_H

#include <dtXmlParserDecorator/dtXmlParserFunctionDecorator.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class dtCurve;

  class vec3dTwoDTwelveRectangleFunctionDecorator : public dtXmlParserFunctionDecorator {
    public:
      dt__CLASSNAME(vec3dTwoDTwelveRectangleFunctionDecorator);    
      vec3dTwoDTwelveRectangleFunctionDecorator();
      virtual ~vec3dTwoDTwelveRectangleFunctionDecorator();
      virtual void buildPart(
        QDomElement const & toBuildP, 
        vectorHandling< constValue * > const * const cValP, 
        vectorHandling< analyticFunction * > const * const depSFunP,
        vectorHandling< analyticFunction * > * sFunP
      ) const;
    private:
      static void reorder(vectorHandling< dtCurve const * > & cc);    
  };
}
#endif	/* VEC3DTWODTWELVERECTANGLEFUNCTIONDECORATOR_H */

