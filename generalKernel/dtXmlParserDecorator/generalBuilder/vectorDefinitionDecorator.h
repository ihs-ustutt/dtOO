#ifndef VECTORDEFINITIONDECORATOR_H
#define	VECTORDEFINITIONDECORATOR_H

#include "vectorDefinitionDecorator.h"
#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

class QDomElement;
class analyticGeometry;
class geometry;
class dtXmlParserDecoratorInterface;
class analyticFunction;
class constValue;

namespace dtOO {
  class vectorDefinitionDecorator : public dtXmlParserDecorator {
    dt__CLASSNAME(vectorDefinitionDecorator);
  public:
    vectorDefinitionDecorator();
    virtual ~vectorDefinitionDecorator();
    virtual void buildPart(
      QDomElement ** toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,  
      vectorHandling< analyticFunction * > const * const sFunP,  
      vectorHandling< analyticGeometry * > const * const depAGeoP,
      vectorHandling< analyticGeometry * > * aGeoP ) const;  
  };
}
#endif	/* VECTORDEFINITIONDECORATOR_H */

