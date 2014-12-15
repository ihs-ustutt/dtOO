#ifndef scaInMap1dTo3dDecorator_H
#define	scaInMap1dTo3dDecorator_H

#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class scaInMap1dTo3dDecorator : public dtXmlParserDecorator {
  public:  
    dt__CLASSNAME(scaInMap1dTo3dDecorator);
    scaInMap1dTo3dDecorator();
    virtual ~scaInMap1dTo3dDecorator();
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
#endif	/* scaInMap1dTo3dDecorator_H */

