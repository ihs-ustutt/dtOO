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
    dt__classOnlyName(bSplineCurve_pointConstructOCCDecorator);
    bSplineCurve_pointConstructOCCDecorator();
    virtual ~bSplineCurve_pointConstructOCCDecorator();
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
#endif	/* BSPLINECURVE_POINTCONSTRUCTOCCDECORATOR_H */

