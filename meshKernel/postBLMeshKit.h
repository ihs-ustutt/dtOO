#ifndef POSTBLMESHKIT_H
#define	POSTBLMESHKIT_H

#include <logMe/dtMacros.h>
#include "boundedVolume.h"

namespace MeshKit {
  class MKCore;
}

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  
  class postBLMeshKit : public boundedVolume {
  public:
    postBLMeshKit();
    virtual ~postBLMeshKit();
    virtual void init( 
      QDomElement const & element,
      baseContainer const * const bC,      
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP,
      vectorHandling< analyticGeometry * > const * const depAGeoP,
      vectorHandling< boundedVolume * > const * const depBVolP
    );    
    virtual void makeGrid(void);
    virtual void makePreGrid(void);
  	virtual vectorHandling< renderInterface * > getRender( void ) const;    
  private:
    std::string _pBLFile;
    ptrHandling< MeshKit::MKCore > _mk;    
  };
}
#endif	/* POSTBLMESHKIT_H */

