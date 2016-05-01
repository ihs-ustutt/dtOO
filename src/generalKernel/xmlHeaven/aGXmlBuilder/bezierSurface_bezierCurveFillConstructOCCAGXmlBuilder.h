#ifndef bezierSurface_bezierCurveFillConstructOCCAGXmlBuilder_H
#define	bezierSurface_bezierCurveFillConstructOCCAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bezierSurface_bezierCurveFillConstructOCCAGXmlBuilder : public aGXmlBuilder {
  public:  
    dt__classOnlyName(bezierSurface_bezierCurveFillConstructOCCAGXmlBuilder);
    bezierSurface_bezierCurveFillConstructOCCAGXmlBuilder();
    virtual ~bezierSurface_bezierCurveFillConstructOCCAGXmlBuilder();
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

#endif	/* bezierSurface_bezierCurveFillConstructOCCAGXmlBuilder_H */