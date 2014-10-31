#ifndef BSPLINECURVE_POINTCONSTRUCTARCLENGTHPARAOCCDECORATOR_H
#define	BSPLINECURVE_POINTCONSTRUCTARCLENGTHPARAOCCDECORATOR_H

#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class pointContainer;
  class vectorContainer;

  class bSplineCurve_pointConstructArcLengthParaOCCDecorator : public dtXmlParserDecorator {
  public:  
    dt__CLASSNAME(bSplineCurve_pointConstructArcLengthParaOCCDecorator);
    bSplineCurve_pointConstructArcLengthParaOCCDecorator();
    virtual ~bSplineCurve_pointConstructArcLengthParaOCCDecorator();
    virtual void buildPart(QDomElement ** toBuildP,
                                  pointContainer * const pointContainerP,
                                  vectorContainer * const vectorContainerP,      
                                  vectorHandling< constValue * > const * const cValP,  
                                  vectorHandling< analyticFunction * > const * const sFunP,  
                                  vectorHandling< analyticGeometry * > const * const depAGeoP,
                                  vectorHandling< analyticGeometry * > * aGeoP ) const;  
  };
}
#endif	/* BSPLINECURVE_POINTCONSTRUCTARCLENGTHPARAOCCDECORATOR_H */

