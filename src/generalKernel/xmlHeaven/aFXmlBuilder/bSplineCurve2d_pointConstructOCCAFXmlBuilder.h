#ifndef bSplineCurve2d_pointConstructOCCAFXmlBuilder_H
#define	bSplineCurve2d_pointConstructOCCAFXmlBuilder_H

#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bSplineCurve2d_pointConstructOCCAFXmlBuilder : public aFXmlBuilder {
  public:
    dt__classOnlyName(bSplineCurve2d_pointConstructOCCAFXmlBuilder);    
    dt__classSelfCreate(bSplineCurve2d_pointConstructOCCAFXmlBuilder);    
    bSplineCurve2d_pointConstructOCCAFXmlBuilder();
    virtual ~bSplineCurve2d_pointConstructOCCAFXmlBuilder();
    virtual void buildPart(
      ::QDomElement const & toBuildP, 
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP, 
      vectorHandling< analyticFunction * > const * const depSFunP,
      vectorHandling< analyticFunction * > * sFunP
    ) const;
  };
}
#endif	/* bSplineCurve2d_pointConstructOCCAFXmlBuilder_H */

