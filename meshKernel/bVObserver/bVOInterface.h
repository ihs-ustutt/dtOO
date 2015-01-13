#ifndef BVOINTERFACE_H
#define	BVOINTERFACE_H

#include <logMe/dtMacros.h>

#include <dtXmlParserDecorator/qtXmlBase.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class baseContainer;
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class boundedVolume;
  
  class bVOInterface {
    public:
      dt__CLASSNAME(bVOInterface);  
      bVOInterface();
      virtual ~bVOInterface();
      virtual void update( void ) = 0;
      virtual void init( 
        QDomElement const & element,
        baseContainer const * const bC,
        vectorHandling< constValue * > const * const cV,
        vectorHandling< analyticFunction * > const * const aF,
        vectorHandling< analyticGeometry * > const * const aG,
        vectorHandling< boundedVolume * > const * const bV,
        boundedVolume * attachTo
      );
    protected:
      boundedVolume * ptrBoundedVolume(void);
    private:
      boundedVolume * _bV;
  };
}
#endif	/* BVOINTERFACE_H */

