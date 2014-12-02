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
  class baseContainer;

  class vec3dTwoDTwelveRectangleFunctionDecorator : public dtXmlParserFunctionDecorator {
    public:
      dt__CLASSNAME(vec3dTwoDTwelveRectangleFunctionDecorator);    
      vec3dTwoDTwelveRectangleFunctionDecorator();
      virtual ~vec3dTwoDTwelveRectangleFunctionDecorator();
      virtual void buildPart(
        QDomElement const & toBuildP, 
        baseContainer * const bC,
        vectorHandling< constValue * > const * const cValP, 
        vectorHandling< analyticFunction * > const * const depSFunP,
        vectorHandling< analyticFunction * > * sFunP
      ) const;
    private:
      static analyticFunction * createCompound( 
        vectorHandling< dtCurve const * > & cL, 
        std::vector< float > const & ll, std::vector< float > const & mm,
        std::vector< float > const & nn, std::vector< float > const & kk 
      );      
      static void reorder(vectorHandling< dtCurve const * > & cc);
  };
}
#endif	/* VEC3DTWODTWELVERECTANGLEFUNCTIONDECORATOR_H */

