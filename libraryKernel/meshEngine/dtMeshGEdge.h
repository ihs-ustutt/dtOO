#ifndef dtMeshGEdge_H
#define	dtMeshGEdge_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtGmshEdge;
  
  class dtMeshGEdge {
  public:
    dt__classOnlyName(dtMeshGEdge);     
    dtMeshGEdge();
    dtMeshGEdge(const dtMeshGEdge& orig);
    virtual ~dtMeshGEdge();
    void operator()( dtGmshEdge * dtge );    
  };
}
#endif	/* dtMeshGEdge_H */

