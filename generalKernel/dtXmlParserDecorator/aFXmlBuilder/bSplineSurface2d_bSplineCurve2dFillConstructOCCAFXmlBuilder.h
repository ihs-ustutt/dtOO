#ifndef bSplineSurface2d_bSplineCurve2dFillConstructOCCAFXmlBuilder_H
#define	bSplineSurface2d_bSplineCurve2dFillConstructOCCAFXmlBuilder_H

#include <dtXmlParserDecorator/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bSplineSurface2d_bSplineCurve2dFillConstructOCCAFXmlBuilder 
    : public aFXmlBuilder {
  public:
    dt__classOnlyName(bSplineSurface2d_bSplineCurve2dFillConstructOCCAFXmlBuilder);    
    bSplineSurface2d_bSplineCurve2dFillConstructOCCAFXmlBuilder();
    virtual ~bSplineSurface2d_bSplineCurve2dFillConstructOCCAFXmlBuilder();
    virtual void buildPart(
      QDomElement const & toBuildP, 
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP, 
      vectorHandling< analyticFunction * > const * const depSFunP,
      vectorHandling< analyticFunction * > * sFunP
    ) const;
  };
}
#endif	/* bSplineSurface2d_bSplineCurve2dFillConstructOCCAFXmlBuilder_H */

