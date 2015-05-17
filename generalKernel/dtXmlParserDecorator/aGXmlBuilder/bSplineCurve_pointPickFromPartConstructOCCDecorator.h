#ifndef BSPLINECURVE_POINTPICKFROMPARTCONSTRUCTOCCDECORATOR_H
#define	BSPLINECURVE_POINTPICKFROMPARTCONSTRUCTOCCDECORATOR_H

#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bSplineCurve_pointPickFromPartConstructOCCDecorator : public dtXmlParserDecorator {
  public:  
    dt__classOnlyName(bSplineCurve_pointPickFromPartConstructOCCDecorator);
    bSplineCurve_pointPickFromPartConstructOCCDecorator();
    virtual ~bSplineCurve_pointPickFromPartConstructOCCDecorator();
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

#endif	/* BSPLINECURVE_POINTPICKFROMPARTCONSTRUCTOCCDECORATOR_H */

