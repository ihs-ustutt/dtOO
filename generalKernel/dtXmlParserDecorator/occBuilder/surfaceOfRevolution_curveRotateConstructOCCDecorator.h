#ifndef SURFACEOFREVOLUTION_CURVEROTATECONSTRUCTOCCDECORATOR_H
#define	SURFACEOFREVOLUTION_CURVEROTATECONSTRUCTOCCDECORATOR_H

#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class pointContainer;
  class vectorContainer;

  class surfaceOfRevolution_curveRotateConstructOCCDecorator : public dtXmlParserDecorator {
  public:  
    dt__CLASSNAME(surfaceOfRevolution_curveRotateConstructOCCDecorator);
    surfaceOfRevolution_curveRotateConstructOCCDecorator();
    virtual ~surfaceOfRevolution_curveRotateConstructOCCDecorator();
    virtual void buildPart(QDomElement ** toBuildP,
                                  pointContainer * const pointContainerP,
                                  vectorContainer * const vectorContainerP,      
                                  vectorHandling< constValue * > const * const cValP,  
                                  vectorHandling< analyticFunction * > const * const sFunP,  
                                  vectorHandling< analyticGeometry * > const * const depAGeoP,
                                  vectorHandling< analyticGeometry * > * aGeoP ) const;  
  };
}
#endif	/* SURFACEOFREVOLUTION_CURVEROTATECONSTRUCTOCCDECORATOR_H */

