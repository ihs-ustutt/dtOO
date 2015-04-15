#ifndef LOFTEDSURFACEWRAPPERDECORATOR_H
#define	LOFTEDSURFACEWRAPPERDECORATOR_H

#include <logMe/dtMacros.h>
#include <dtXmlParserDecorator/dtXmlParserDecorator.h>

namespace dtOO {
  class loftedSurfaceWrapperDecorator : public dtXmlParserDecorator {
  public:
    dt__classOnlyName(loftedSurfaceWrapperDecorator);
    loftedSurfaceWrapperDecorator();
    virtual ~loftedSurfaceWrapperDecorator();
    virtual void buildPart(QDomElement ** toBuildP,
                                  pointContainer * const pointContainerP,
                                  vectorContainer * const vectorContainerP,    
                                  vectorHandling< constValue * > const * const cValP,  
                                  vectorHandling< analyticFunction * > const * const sFunP,
                                  vectorHandling< analyticGeometry * > const * const depAGeoP, 
                                  vectorHandling< analyticGeometry * > * aGeoP ) const;       
  };
}
#endif	/* LOFTEDSURFACEWRAPPERDECORATOR_H */