#ifndef SPLINE3DTOBLENDEDSURFACEDECORATOR_H
#define	SPLINE3DTOBLENDEDSURFACEDECORATOR_H

#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

class QDomElement;
class analyticGeometry;
class geometry;
class dtXmlParserDecoratorInterface;
class analyticFunction;
class constValue;
class pointContainer;
class vectorContainer;

namespace dtOO {
  class spline3dToBlendedSurfaceDecorator : public dtXmlParserDecorator {
  public:  
    dt__CLASSNAME(spline3dToBlendedSurfaceDecorator);
    spline3dToBlendedSurfaceDecorator();
    virtual ~spline3dToBlendedSurfaceDecorator();
    virtual void buildPart(QDomElement ** toBuildP,
                                  pointContainer * const pointContainerP,
                                  vectorContainer * const vectorContainerP,      
                                  vectorHandling< constValue * > const * const cValP,  
                                  vectorHandling< analyticFunction * > const * const sFunP,  
                                  vectorHandling< analyticGeometry * > const * const depAGeoP,
                                  vectorHandling< analyticGeometry * > * aGeoP ) const;  
  };
}

#endif	/* SPLINE3DTOBLENDEDSURFACEDECORATOR_H */

