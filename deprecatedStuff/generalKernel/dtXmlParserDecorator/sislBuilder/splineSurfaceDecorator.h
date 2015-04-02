#ifndef SPLINESURFACEDECORATOR_H
#define	SPLINESURFACEDECORATOR_H

#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

class QDomElement;
class analyticGeometry;
class analyticFunction;
class constValue;
class pointContainer;
class vectorContainer;

namespace dtOO {
  class splineSurfaceDecorator : public dtXmlParserDecorator {
  public:  
    dt__classOnlyName(splineSurfaceDecorator);
    splineSurfaceDecorator();
    virtual ~splineSurfaceDecorator();
    virtual void buildPart(QDomElement ** toBuildP,
                                  pointContainer * const pointContainerP,
                                  vectorContainer * const vectorContainerP,      
                                  vectorHandling< constValue * > const * const cValP,  
                                  vectorHandling< analyticFunction * > const * const sFunP,  
                                  vectorHandling< analyticGeometry * > const * const depAGeoP, 
                                  vectorHandling< analyticGeometry * > * aGeoP ) const;  
  };
}

#endif	/* SPLINESURFACEDECORATOR_H */

