#ifndef ANALYTICGEOMETRYDECORATOR_H
#define	ANALYTICGEOMETRYDECORATOR_H

#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

class QDomElement;
class analyticGeometry;
class geometry;
class dtXmlParserDecoratorInterface;
class analyticFunction;
class constValue;
class baseContainer;

namespace dtOO {
  class analyticGeometryDecorator : public dtXmlParserDecorator {
  public:  
    dt__CLASSNAME(analyticGeometryDecorator);
    analyticGeometryDecorator();
    virtual ~analyticGeometryDecorator();
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
#endif	/* ANALYTICGEOMETRYDECORATOR_H */

