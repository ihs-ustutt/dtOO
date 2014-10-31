#ifndef vec3dInMap3dTo3dDecorator_H
#define	vec3dInMap3dTo3dDecorator_H

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

  class vec3dInMap3dTo3dDecorator : public dtXmlParserDecorator {
  public:  
    dt__CLASSNAME(vec3dInMap3dTo3dDecorator);
    vec3dInMap3dTo3dDecorator();
    virtual ~vec3dInMap3dTo3dDecorator();
    virtual void buildPart(
      QDomElement ** toBuildP,
      pointContainer * const pointContainerP,
      vectorContainer * const vectorContainerP,    
      vectorHandling< constValue * > const * const cValP,  
      vectorHandling< analyticFunction * > const * const sFunP,
      vectorHandling< analyticGeometry * > const * const depAGeoP,
      vectorHandling< analyticGeometry * > * aGeoP 
    ) const;   
    virtual void buildPartCompound(
      QDomElement ** toBuildP,
      pointContainer * const pointContainerP,
      vectorContainer * const vectorContainerP,    
      vectorHandling< constValue * > const * const cValP,  
      vectorHandling< analyticFunction * > const * const sFunP,
      vectorHandling< analyticGeometry * > const * const depAGeoP,
      vectorHandling< analyticGeometry * > * aGeoP 
    ) const;       
  };
}
#endif	/* vec3dInMap3dTo3dDecorator_H */

