#ifndef SURFACEOFREVOLUTION_CURVEROTATECONSTRUCTOCCDECORATOR_H
#define	SURFACEOFREVOLUTION_CURVEROTATECONSTRUCTOCCDECORATOR_H

#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class surfaceOfRevolution_curveRotateConstructOCCDecorator : public dtXmlParserDecorator {
  public:  
    dt__classOnlyName(surfaceOfRevolution_curveRotateConstructOCCDecorator);
    surfaceOfRevolution_curveRotateConstructOCCDecorator();
    virtual ~surfaceOfRevolution_curveRotateConstructOCCDecorator();
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
#endif	/* SURFACEOFREVOLUTION_CURVEROTATECONSTRUCTOCCDECORATOR_H */

