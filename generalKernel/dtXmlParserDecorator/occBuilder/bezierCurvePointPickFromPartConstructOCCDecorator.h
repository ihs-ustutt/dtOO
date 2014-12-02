#ifndef BEZIERCURVEPOINTPICKFROMPARTCONSTRUCTOCCDECORATOR_H
#define	BEZIERCURVEPOINTPICKFROMPARTCONSTRUCTOCCDECORATOR_H

#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class dtXmlParserDecoratorInterface;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bezierCurvePointPickFromPartConstructOCCDecorator : public dtXmlParserDecorator {
  public:  
    dt__CLASSNAME(bezierCurvePointPickFromPartConstructOCCDecorator);
    bezierCurvePointPickFromPartConstructOCCDecorator();
    virtual ~bezierCurvePointPickFromPartConstructOCCDecorator();
    virtual void buildPart(
      QDomElement ** toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,  
      vectorHandling< analyticFunction * > const * const sFunP,  
      vectorHandling< analyticGeometry * > const * const depAGeoP,
      vectorHandling< analyticGeometry * > * aGeoP 
    ) const;
  };
}

#endif	/* BEZIERCURVEPOINTPICKFROMPARTCONSTRUCTOCCDECORATOR_H */

