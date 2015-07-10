#ifndef BVOINTERFACE_H
#define	BVOINTERFACE_H

#include <logMe/dtMacros.h>

#include <xmlHeaven/qtXmlBase.h>
#include <interfaceHeaven/vectorHandling.h>
#include <interfaceHeaven/optionHandling.h>

namespace dtOO {
  class baseContainer;
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class boundedVolume;
  
  class bVOInterface : public optionHandling {
    public:
      dt__classOnlyName(bVOInterface);  
      bVOInterface();
      virtual ~bVOInterface();
      virtual void preUpdate( void );
      virtual void postUpdate( void );
      virtual void init( 
        ::QDomElement const & element,
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

