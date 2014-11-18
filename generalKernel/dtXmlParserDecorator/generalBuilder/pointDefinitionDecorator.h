#ifndef POINTDEFINITIONDECORATOR_H
#define	POINTDEFINITIONDECORATOR_H

#include "pointDefinitionDecorator.h"
#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

class QDomElement;
class analyticGeometry;
class dtXmlParserDecoratorInterface;
class analyticFunction;
class constValue;
class baseContainer;
class vectorContainer;

namespace dtOO {
  class pointDefinitionDecorator : public dtXmlParserDecorator {
    dt__CLASSNAME(pointDefinitionDecorator);
  public:
    pointDefinitionDecorator();
    virtual ~pointDefinitionDecorator();
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
#endif	/* POINTDEFINITIONDECORATOR_H */

