#ifndef BSPLINECURVE_POINTINTERPOLATECONSTRUCTOCCDECORATOR_H
#define	BSPLINECURVE_POINTINTERPOLATECONSTRUCTOCCDECORATOR_H

#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bSplineCurve_pointInterpolateConstructOCCDecorator : public dtXmlParserDecorator {
  public:  
    dt__classOnlyName(bSplineCurve_pointInterpolateConstructOCCDecorator);
    bSplineCurve_pointInterpolateConstructOCCDecorator();
    virtual ~bSplineCurve_pointInterpolateConstructOCCDecorator();
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
#endif	/* BSPLINECURVE_POINTINTERPOLATECONSTRUCTOCCDECORATOR_H */

