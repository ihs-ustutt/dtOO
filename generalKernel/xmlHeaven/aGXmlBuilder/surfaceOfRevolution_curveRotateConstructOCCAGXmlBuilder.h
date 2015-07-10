#ifndef surfaceOfRevolution_curveRotateConstructOCCAGXmlBuilder_H
#define	surfaceOfRevolution_curveRotateConstructOCCAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class surfaceOfRevolution_curveRotateConstructOCCAGXmlBuilder : public aGXmlBuilder {
  public:  
    dt__classOnlyName(surfaceOfRevolution_curveRotateConstructOCCAGXmlBuilder);
    surfaceOfRevolution_curveRotateConstructOCCAGXmlBuilder();
    virtual ~surfaceOfRevolution_curveRotateConstructOCCAGXmlBuilder();
    virtual void buildPart(
      ::QDomElement const & toBuild,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cV,  
      vectorHandling< analyticFunction * > const * const aF,  
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< analyticGeometry * > * result
    ) const;  
  };
}
#endif	/* surfaceOfRevolution_curveRotateConstructOCCAGXmlBuilder_H */

