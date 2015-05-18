#ifndef bSplineSurface_bSplineCurveFillConstructOCCAGXmlBuilder_H
#define	bSplineSurface_bSplineCurveFillConstructOCCAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bSplineSurface_bSplineCurveFillConstructOCCAGXmlBuilder : public aGXmlBuilder {
    public:  
      dt__classOnlyName(bSplineSurface_bSplineCurveFillConstructOCCAGXmlBuilder);
      bSplineSurface_bSplineCurveFillConstructOCCAGXmlBuilder();
      virtual ~bSplineSurface_bSplineCurveFillConstructOCCAGXmlBuilder();
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

#endif	/* bSplineSurface_bSplineCurveFillConstructOCCAGXmlBuilder_H */