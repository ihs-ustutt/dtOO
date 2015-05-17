#ifndef bezierCurve_pointConstructOCCAGXmlBuilder_H
#define	bezierCurve_pointConstructOCCAGXmlBuilder_H

#include <dtXmlParserDecorator/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bezierCurve_pointConstructOCCAGXmlBuilder : public aGXmlBuilder {
  public:  
    dt__classOnlyName(bezierCurve_pointConstructOCCAGXmlBuilder);
    bezierCurve_pointConstructOCCAGXmlBuilder();
    virtual ~bezierCurve_pointConstructOCCAGXmlBuilder();
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
#endif	/* bezierCurve_pointConstructOCCAGXmlBuilder_H */

