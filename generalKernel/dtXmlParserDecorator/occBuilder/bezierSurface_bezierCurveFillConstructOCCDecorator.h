#ifndef BEZIERSURFACE_BEZIERCURVEFILLCONSTRUCTOCCDECORATOR_H
#define	BEZIERSURFACE_BEZIERCURVEFILLCONSTRUCTOCCDECORATOR_H

#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bezierSurface_bezierCurveFillConstructOCCDecorator : public dtXmlParserDecorator {
  public:  
    dt__classOnlyName(bezierSurface_bezierCurveFillConstructOCCDecorator);
    bezierSurface_bezierCurveFillConstructOCCDecorator();
    virtual ~bezierSurface_bezierCurveFillConstructOCCDecorator();
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

#endif	/* BEZIERSURFACE_BEZIERCURVEFILLCONSTRUCTOCCDECORATOR_H */