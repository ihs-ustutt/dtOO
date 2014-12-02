#ifndef BSPLINECURVE_POINTCONSTRUCTOCCDECORATOR_H
#define	BSPLINECURVE_POINTCONSTRUCTOCCDECORATOR_H

#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bSplineCurve_pointConstructOCCDecorator : public dtXmlParserDecorator {
  public:  
    dt__CLASSNAME(bSplineCurve_pointConstructOCCDecorator);
    bSplineCurve_pointConstructOCCDecorator();
    virtual ~bSplineCurve_pointConstructOCCDecorator();
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
#endif	/* BSPLINECURVE_POINTCONSTRUCTOCCDECORATOR_H */

