#ifndef BSPLINECURVE_CURVECONNECTCONSTRUCTOCCDECORATOR_H
#define	BSPLINECURVE_CURVECONNECTCONSTRUCTOCCDECORATOR_H

#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bSplineCurve_curveConnectConstructOCCDecorator : public dtXmlParserDecorator {
  public:  
    dt__classOnlyName(bSplineCurve_curveConnectConstructOCCDecorator);
    bSplineCurve_curveConnectConstructOCCDecorator();
    virtual ~bSplineCurve_curveConnectConstructOCCDecorator();
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
#endif	/* BSPLINECURVE_CURVECONNECTCONSTRUCTOCCDECORATOR_H */

