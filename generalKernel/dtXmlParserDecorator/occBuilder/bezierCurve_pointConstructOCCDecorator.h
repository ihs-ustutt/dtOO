#ifndef BEZIERCURVE_POINTCONSTRUCTOCCDECORATOR_H
#define	BEZIERCURVE_POINTCONSTRUCTOCCDECORATOR_H

#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class pointContainer;
  class vectorContainer;

  class bezierCurve_pointConstructOCCDecorator : public dtXmlParserDecorator {
  public:  
    dt__CLASSNAME(bezierCurve_pointConstructOCCDecorator);
    bezierCurve_pointConstructOCCDecorator();
    virtual ~bezierCurve_pointConstructOCCDecorator();
    virtual void buildPart(QDomElement ** toBuildP,
                                  pointContainer * const pointContainerP,
                                  vectorContainer * const vectorContainerP,      
                                  vectorHandling< constValue * > const * const cValP,  
                                  vectorHandling< analyticFunction * > const * const sFunP,  
                                  vectorHandling< analyticGeometry * > const * const depAGeoP,
                                  vectorHandling< analyticGeometry * > * aGeoP ) const;  
  };
}
#endif	/* BEZIERCURVE_POINTCONSTRUCTOCCDECORATOR_H */

