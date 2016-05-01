#ifndef bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder_H
#define	bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder : public aGXmlBuilder {
  public:  
    dt__classOnlyName(bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder);
    bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder();
    virtual ~bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder();
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
#endif	/* bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder_H */

