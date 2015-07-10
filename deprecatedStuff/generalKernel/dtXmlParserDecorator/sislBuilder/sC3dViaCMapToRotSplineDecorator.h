#ifndef SC3DVIACMAPTOROTSPLINEDECORATOR_H
#define	SC3DVIACMAPTOROTSPLINEDECORATOR_H

#include <logMe/dtMacros.h>
#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class pointContainer;
  class vectorContainer;
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class dtXmlParserDecoratorInterface;
  
  class sC3dViaCMapToRotSplineDecorator : public dtXmlParserDecorator {
  public:
    dt__classOnlyName(sC3dViaCMapToRotSplineDecorator);
    sC3dViaCMapToRotSplineDecorator();
    virtual void buildPart(::QDomElement ** toBuildP,
                                  pointContainer * const pointContainerP,
                                  vectorContainer * const vectorContainerP,    
                                  vectorHandling< constValue * > const * const cValP,  
                                  vectorHandling< analyticFunction * > const * const sFunP,
                                  vectorHandling< analyticGeometry * > const * const depAGeoP,
                                  vectorHandling< analyticGeometry * > * aGeoP ) const;     
    virtual ~sC3dViaCMapToRotSplineDecorator();
  };
}
#endif	/* SC3DVIACMAPTOROTSPLINEDECORATOR_H */

