#ifndef bSplineCurve_pointConstructArcLengthParaOCCAGXmlBuilder_H
#define	bSplineCurve_pointConstructArcLengthParaOCCAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bSplineCurve_pointConstructArcLengthParaOCCAGXmlBuilder : public aGXmlBuilder {
  public:  
    dt__classOnlyName(bSplineCurve_pointConstructArcLengthParaOCCAGXmlBuilder);
    bSplineCurve_pointConstructArcLengthParaOCCAGXmlBuilder();
    virtual ~bSplineCurve_pointConstructArcLengthParaOCCAGXmlBuilder();
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
#endif	/* bSplineCurve_pointConstructArcLengthParaOCCAGXmlBuilder_H */