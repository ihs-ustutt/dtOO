#ifndef BEZIERCURVEPOINTPICKFROMPARTCONSTRUCTOCCDECORATOR_H
#define	BEZIERCURVEPOINTPICKFROMPARTCONSTRUCTOCCDECORATOR_H

#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bezierCurvePointPickFromPartConstructOCCDecorator : public dtXmlParserDecorator {
  public:  
    dt__classOnlyName(bezierCurvePointPickFromPartConstructOCCDecorator);
    bezierCurvePointPickFromPartConstructOCCDecorator();
    virtual ~bezierCurvePointPickFromPartConstructOCCDecorator();
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

#endif	/* BEZIERCURVEPOINTPICKFROMPARTCONSTRUCTOCCDECORATOR_H */

