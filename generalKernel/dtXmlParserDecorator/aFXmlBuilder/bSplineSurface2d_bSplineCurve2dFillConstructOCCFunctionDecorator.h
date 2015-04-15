#ifndef BSPLINESURFACE2D_BSPLINECURVE2DFILLCONSTRUCTOCCFUNCTIONDECORATOR_H
#define	BSPLINESURFACE2D_BSPLINECURVE2DFILLCONSTRUCTOCCFUNCTIONDECORATOR_H

#include <dtXmlParserDecorator/dtXmlParserFunctionDecorator.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bSplineSurface2d_bSplineCurve2dFillConstructOCCFunctionDecorator 
    : public dtXmlParserFunctionDecorator {
  public:
    dt__classOnlyName(bSplineSurface2d_bSplineCurve2dFillConstructOCCFunctionDecorator);    
    bSplineSurface2d_bSplineCurve2dFillConstructOCCFunctionDecorator();
    virtual ~bSplineSurface2d_bSplineCurve2dFillConstructOCCFunctionDecorator();
    virtual void buildPart(
      QDomElement const & toBuildP, 
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP, 
      vectorHandling< analyticFunction * > const * const depSFunP,
      vectorHandling< analyticFunction * > * sFunP
    ) const;
  };
}
#endif	/* BSPLINESURFACE2D_BSPLINECURVE2DFILLCONSTRUCTOCCFUNCTIONDECORATOR_H */

