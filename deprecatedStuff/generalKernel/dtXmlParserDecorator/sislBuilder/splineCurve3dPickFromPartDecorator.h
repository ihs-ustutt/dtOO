#ifndef SPLINECURVE3DPICKFROMPARTDECORATOR_H
#define	SPLINECURVE3DPICKFROMPARTDECORATOR_H

#include <logMe/dtMacros.h>
#include <dtXmlParserDecorator/dtXmlParserDecorator.h>

namespace dtOO {
  class splineCurve3dPickFromPartDecorator : public dtXmlParserDecorator {
  public:
    dt__classOnlyName(splineCurve3dPickFromPartDecorator);
    splineCurve3dPickFromPartDecorator();
    virtual ~splineCurve3dPickFromPartDecorator();
    virtual void buildPart(::QDomElement ** toBuildP,
                                  pointContainer * const pointContainerP,
                                  vectorContainer * const vectorContainerP,    
                                  vectorHandling< constValue * > const * const cValP,  
                                  vectorHandling< analyticFunction * > const * const sFunP,
                                  vectorHandling< analyticGeometry * > const * const depAGeoP,  
                                  vectorHandling< analyticGeometry * > * aGeoP ) const;       
  };
}
#endif	/* SPLINECURVE3DPICKFROMPARTDECORATOR_H */