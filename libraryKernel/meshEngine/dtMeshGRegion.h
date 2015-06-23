#ifndef DTMESHGREGION_H
#define	DTMESHGREGION_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtGmshRegion;
  class dtGmshFace;
  
  class dtMeshGRegion {
  public:
    dt__classOnlyName(dtMeshGRegion);     
    dtMeshGRegion();
    dtMeshGRegion(const dtMeshGRegion& orig);
    virtual ~dtMeshGRegion();
    void operator()( dtGmshRegion * dtgr );    
  private:
    void createPyramids( dtGmshRegion * dtgr );
  };
}
#endif	/* DTMESHGREGION_H */

