#ifndef BVOINTERFACE_H
#define	BVOINTERFACE_H

#include <logMe/dtMacros.h>

#include <dtXmlParserDecorator/qtXmlBase.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class boundedVolume;
  
  class bVOInterface : public qtXmlBase {
    public:
      dt__CLASSNAME(bVOInterface);  
      bVOInterface();
      virtual ~bVOInterface();
      virtual void update( void ) = 0;
      virtual void init( 
        QDomElement const & element,
        vectorHandling< constValue * > const * const cValP,
        vectorHandling< analyticFunction * > const * const sFunP,
        vectorHandling< analyticGeometry * > const * const depAGeoP,
        vectorHandling< boundedVolume * > const * const depBVolP,
        boundedVolume * bV
      );
    protected:
      boundedVolume * _bV;
  };
}
#endif	/* BVOINTERFACE_H */

