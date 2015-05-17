#ifndef BSPLINESURFACE_BSPLINECURVEFILLCONSTRUCTOCCDECORATOR_H
#define	BSPLINESURFACE_BSPLINECURVEFILLCONSTRUCTOCCDECORATOR_H

#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bSplineSurface_bSplineCurveFillConstructOCCDecorator : public dtXmlParserDecorator {
    public:  
      dt__classOnlyName(bSplineSurface_bSplineCurveFillConstructOCCDecorator);
      bSplineSurface_bSplineCurveFillConstructOCCDecorator();
      virtual ~bSplineSurface_bSplineCurveFillConstructOCCDecorator();
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

#endif	/* BSPLINESURFACE_BSPLINECURVEFILLCONSTRUCTOCCDECORATOR_H */