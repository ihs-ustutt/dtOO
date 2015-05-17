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
      QDomElement const & toBuild,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cV,  
      vectorHandling< analyticFunction * > const * const aF,  
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< analyticGeometry * > * result 
    ) const;
  };
}

#endif	/* BEZIERSURFACE_BEZIERCURVEFILLCONSTRUCTOCCDECORATOR_H */