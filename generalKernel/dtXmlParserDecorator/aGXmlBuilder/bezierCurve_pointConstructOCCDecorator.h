#ifndef BEZIERCURVE_POINTCONSTRUCTOCCDECORATOR_H
#define	BEZIERCURVE_POINTCONSTRUCTOCCDECORATOR_H

#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bezierCurve_pointConstructOCCDecorator : public dtXmlParserDecorator {
  public:  
    dt__classOnlyName(bezierCurve_pointConstructOCCDecorator);
    bezierCurve_pointConstructOCCDecorator();
    virtual ~bezierCurve_pointConstructOCCDecorator();
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
#endif	/* BEZIERCURVE_POINTCONSTRUCTOCCDECORATOR_H */

