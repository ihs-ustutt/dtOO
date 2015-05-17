#ifndef bSplineCurve_pointConstructOCCAGXmlBuilder_H
#define	bSplineCurve_pointConstructOCCAGXmlBuilder_H

#include <dtXmlParserDecorator/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bSplineCurve_pointConstructOCCAGXmlBuilder : public aGXmlBuilder {
  public:  
    dt__classOnlyName(bSplineCurve_pointConstructOCCAGXmlBuilder);
    bSplineCurve_pointConstructOCCAGXmlBuilder();
    virtual ~bSplineCurve_pointConstructOCCAGXmlBuilder();
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
#endif	/* bSplineCurve_pointConstructOCCAGXmlBuilder_H */

