#ifndef MAP3DTO3DCOMPOUNDVOLUME_H
#define	MAP3DTO3DCOMPOUNDVOLUME_H

#include <logMe/dtMacros.h>
#include "boundedVolume.h"
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class dtGmshModel;
   
  class map3dTo3dCompoundVolume : public boundedVolume {
  public:
    map3dTo3dCompoundVolume();
    virtual ~map3dTo3dCompoundVolume();
    virtual void init( 
      QDomElement const & element,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP,
      vectorHandling< analyticGeometry * > const * const depAGeoP,
      vectorHandling< boundedVolume * > const * const depBVolP
    );    
    virtual void makeGrid(void);
    virtual void makePreGrid(void);
  	virtual vectorHandling< renderInterface * > getRender( void ) const;
  private:
    ptrHandling< dtGmshModel > _gm;
  };
}
#endif	/* MAP3DTO3DCOMPOUNDVOLUME_H */

