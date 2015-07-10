#ifndef dtMeshGFace_H
#define	dtMeshGFace_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtGmshFace;
  
  class dtMeshGFace {
  public:
    dt__classOnlyName(dtMeshGFace);     
    dtMeshGFace();
    dtMeshGFace(const dtMeshGFace& orig);
    virtual ~dtMeshGFace();
    void operator()( dtGmshFace * dtgr );    
  };
}
#endif	/* dtMeshGFace_H */

