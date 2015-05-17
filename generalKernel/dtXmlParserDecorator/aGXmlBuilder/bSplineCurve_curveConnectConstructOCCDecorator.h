#ifndef BSPLINECURVE_CURVECONNECTCONSTRUCTOCCDECORATOR_H
#define	BSPLINECURVE_CURVECONNECTCONSTRUCTOCCDECORATOR_H

#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bSplineCurve_curveConnectConstructOCCDecorator : public dtXmlParserDecorator {
  public:  
    dt__classOnlyName(bSplineCurve_curveConnectConstructOCCDecorator);
    bSplineCurve_curveConnectConstructOCCDecorator();
    virtual ~bSplineCurve_curveConnectConstructOCCDecorator();
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
#endif	/* BSPLINECURVE_CURVECONNECTCONSTRUCTOCCDECORATOR_H */

