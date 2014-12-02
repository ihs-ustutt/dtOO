#ifndef rotatingMap2dTo3dDecorator_H
#define	rotatingMap2dTo3dDecorator_H

#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class rotatingMap2dTo3dDecorator : public dtXmlParserDecorator {
  public:  
    dt__CLASSNAME(rotatingMap2dTo3dDecorator);
    rotatingMap2dTo3dDecorator();
    virtual ~rotatingMap2dTo3dDecorator();
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
#endif	/* rotatingMap2dTo3dDecorator_H */

