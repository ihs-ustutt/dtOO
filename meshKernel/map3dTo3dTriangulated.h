#ifndef MAP3DTO3DTRIANGULATED_H
#define	MAP3DTO3DTRIANGULATED_H

#include <logMe/dtMacros.h>
#include "boundedVolume.h"
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class baseContainer;
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class dtGmshModel;
  class map3dTo3d;
  
  class map3dTo3dTriangulated : public boundedVolume {
  public:
    dt__CLASSSTD(map3dTo3dTriangulated, boundedVolume);
    map3dTo3dTriangulated();
    virtual ~map3dTo3dTriangulated();
    virtual void init( 
      QDomElement const & element,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< boundedVolume * > const * const bV
    );    
    virtual void makeGrid(void);
    virtual void makePreGrid(void);
  	virtual vectorHandling< renderInterface * > getRender( void ) const;
    dtGmshModel * refDtGmshModel(void);    
  private:
    dt__pH(map3dTo3d) _m3d;    
    ptrHandling< dtGmshModel > _gm;    
  };
}

#endif	/* MAP3DTO3DTRIANGULATED_H */

