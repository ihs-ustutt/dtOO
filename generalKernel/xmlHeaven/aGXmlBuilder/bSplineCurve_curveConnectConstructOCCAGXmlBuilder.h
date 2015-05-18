#ifndef bSplineCurve_curveConnectConstructOCCAGXmlBuilder_H
#define	bSplineCurve_curveConnectConstructOCCAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bSplineCurve_curveConnectConstructOCCAGXmlBuilder : public aGXmlBuilder {
  public:  
    dt__classOnlyName(bSplineCurve_curveConnectConstructOCCAGXmlBuilder);
    bSplineCurve_curveConnectConstructOCCAGXmlBuilder();
    virtual ~bSplineCurve_curveConnectConstructOCCAGXmlBuilder();
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
#endif	/* bSplineCurve_curveConnectConstructOCCAGXmlBuilder_H */

