#ifndef vec3dTwoDTwelveRectangleAFXmlBuilder_H
#define	vec3dTwoDTwelveRectangleAFXmlBuilder_H

#include <dtXmlParserDecorator/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class dtCurve;
  class baseContainer;

  class vec3dTwoDTwelveRectangleAFXmlBuilder : public aFXmlBuilder {
    public:
      dt__classOnlyName(vec3dTwoDTwelveRectangleAFXmlBuilder);    
      vec3dTwoDTwelveRectangleAFXmlBuilder();
      virtual ~vec3dTwoDTwelveRectangleAFXmlBuilder();
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
#endif	/* vec3dTwoDTwelveRectangleAFXmlBuilder_H */

