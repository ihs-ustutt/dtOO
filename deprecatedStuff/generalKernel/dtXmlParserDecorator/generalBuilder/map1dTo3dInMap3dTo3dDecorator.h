#ifndef map1dTo3dInMap3dTo3dDecorator_H
#define	map1dTo3dInMap3dTo3dDecorator_H

#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class analyticGeometry;
  class dtXmlParserDecoratorInterface;
  class analyticFunction;
  class constValue;
  class pointContainer;
  class vectorContainer;

  class map1dTo3dInMap3dTo3dDecorator : public dtXmlParserDecorator {
  public:  
    dt__classOnlyName(map1dTo3dInMap3dTo3dDecorator);
    map1dTo3dInMap3dTo3dDecorator();
    virtual ~map1dTo3dInMap3dTo3dDecorator();
    virtual void buildPart(::QDomElement ** toBuildP,
                                  pointContainer * const pointContainerP,
                                  vectorContainer * const vectorContainerP,    
                                  vectorHandling< constValue * > const * const cValP,  
                                  vectorHandling< analyticFunction * > const * const sFunP,
                                  vectorHandling< analyticGeometry * > const * const depAGeoP,
                                  vectorHandling< analyticGeometry * > * aGeoP ) const;   
  };
}
#endif	/* map1dTo3dInMap3dTo3dDecorator_H */

