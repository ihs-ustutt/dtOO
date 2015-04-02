#ifndef SPLINE3DDECORATOR_H
#define	SPLINE3DDECORATOR_H

#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class pointContainer;
  class vectorContainer;

  class spline3dDecorator : public dtXmlParserDecorator {
  public:  
    dt__classOnlyName(spline3dDecorator);
    spline3dDecorator();
    virtual ~spline3dDecorator();
    virtual void buildPart(QDomElement ** toBuildP,
                                  pointContainer * const pointContainerP,
                                  vectorContainer * const vectorContainerP,      
                                  vectorHandling< constValue * > const * const cValP,  
                                  vectorHandling< analyticFunction * > const * const sFunP,  
                                  vectorHandling< analyticGeometry * > const * const depAGeoP,
                                  vectorHandling< analyticGeometry * > * aGeoP ) const;  
  };
}
#endif	/* SPLINE3DDECORATOR_H */

