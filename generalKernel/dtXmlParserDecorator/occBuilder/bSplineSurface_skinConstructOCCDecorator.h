#ifndef BPLINESURFACE_SKINCONSTRUCTOCCDECORATOR_H
#define	BPLINESURFACE_SKINCONSTRUCTOCCDECORATOR_H

#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class pointContainer;
  class vectorContainer;

  class bSplineSurface_skinConstructOCCDecorator : public dtXmlParserDecorator {
  public:  
    dt__CLASSNAME(bSplineSurface_skinConstructOCCDecorator);
    bSplineSurface_skinConstructOCCDecorator();
    virtual ~bSplineSurface_skinConstructOCCDecorator();
    virtual void buildPart(QDomElement ** toBuildP,
                                  pointContainer * const pointContainerP,
                                  vectorContainer * const vectorContainerP,      
                                  vectorHandling< constValue * > const * const cValP,  
                                  vectorHandling< analyticFunction * > const * const sFunP,  
                                  vectorHandling< analyticGeometry * > const * const depAGeoP,
                                  vectorHandling< analyticGeometry * > * aGeoP ) const;  
  };
}
#endif	/* BPLINESURFACE_SKINCONSTRUCTOCCDECORATOR_H */

