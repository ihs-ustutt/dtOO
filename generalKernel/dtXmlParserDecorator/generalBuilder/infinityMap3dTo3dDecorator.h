#ifndef infinityMap3dTo3dDecorator_H
#define	infinityMap3dTo3dDecorator_H

#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class infinityMap3dTo3dDecorator : public dtXmlParserDecorator {
  public:  
    dt__CLASSNAME(infinityMap3dTo3dDecorator);
    infinityMap3dTo3dDecorator();
    virtual ~infinityMap3dTo3dDecorator();
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
#endif	/* infinityMap3dTo3dDecorator_H */

