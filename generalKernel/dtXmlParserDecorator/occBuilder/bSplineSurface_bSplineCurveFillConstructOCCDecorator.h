#ifndef BSPLINESURFACE_BSPLINECURVEFILLCONSTRUCTOCCDECORATOR_H
#define	BSPLINESURFACE_BSPLINECURVEFILLCONSTRUCTOCCDECORATOR_H

#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class pointContainer;
  class vectorContainer;

  class bSplineSurface_bSplineCurveFillConstructOCCDecorator : public dtXmlParserDecorator {
  public:  
    dt__CLASSNAME(bSplineSurface_bSplineCurveFillConstructOCCDecorator);
    bSplineSurface_bSplineCurveFillConstructOCCDecorator();
    virtual ~bSplineSurface_bSplineCurveFillConstructOCCDecorator();
    virtual void buildPart(QDomElement ** toBuildP,
                                  pointContainer * const pointContainerP,
                                  vectorContainer * const vectorContainerP,      
                                  vectorHandling< constValue * > const * const cValP,  
                                  vectorHandling< analyticFunction * > const * const sFunP,  
                                  vectorHandling< analyticGeometry * > const * const depAGeoP,
                                  vectorHandling< analyticGeometry * > * aGeoP ) const;  
  };
}

#endif	/* BSPLINESURFACE_BSPLINECURVEFILLCONSTRUCTOCCDECORATOR_H */