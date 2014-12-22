#ifndef BASECONTAINERDECORATOR_H
#define	BASECONTAINERDECORATOR_H

#include "baseContainerDecorator.h"
#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;
  class vectorContainer;

  class baseContainerDecorator : public dtXmlParserDecorator {
    dt__CLASSNAME(baseContainerDecorator);
  public:
    baseContainerDecorator();
    virtual ~baseContainerDecorator();
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
#endif	/* BASECONTAINERDECORATOR_H */

