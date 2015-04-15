#ifndef vec2dInMap2dTo3dDecorator_H
#define	vec2dInMap2dTo3dDecorator_H

#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class vec2dInMap2dTo3dDecorator : public dtXmlParserDecorator {
  public:  
    dt__classOnlyName(vec2dInMap2dTo3dDecorator);
    vec2dInMap2dTo3dDecorator();
    virtual ~vec2dInMap2dTo3dDecorator();
    virtual void buildPart(
      QDomElement ** toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,  
      vectorHandling< analyticFunction * > const * const sFunP,  
      vectorHandling< analyticGeometry * > const * const depAGeoP,
      vectorHandling< analyticGeometry * > * aGeoP 
    ) const;
  };
}
#endif	/* vec2dInMap2dTo3dDecorator_H */

