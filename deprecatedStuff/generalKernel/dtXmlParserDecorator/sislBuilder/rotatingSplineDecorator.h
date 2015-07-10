#ifndef ROTATINGSPLINEDECORATOR_H
#define	ROTATINGSPLINEDECORATOR_H

#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class pointContainer;
  class vectorContainer;

  class rotatingSplineDecorator : public dtXmlParserDecorator {
  public:  
    dt__classOnlyName(rotatingSplineDecorator);
    rotatingSplineDecorator();
    virtual ~rotatingSplineDecorator();
    virtual void buildPart(::QDomElement ** toBuildP,
                                  pointContainer * const pointContainerP,
                                  vectorContainer * const vectorContainerP,    
                                  vectorHandling< constValue * > const * const cValP,  
                                  vectorHandling< analyticFunction * > const * const sFunP,
                                  vectorHandling< analyticGeometry * > const * const depAGeoP,
                                  vectorHandling< analyticGeometry * > * aGeoP ) const;   
  };
}
#endif	/* ROTATINGSPLINEDECORATOR_H */

