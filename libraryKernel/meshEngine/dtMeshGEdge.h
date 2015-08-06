#ifndef dtMeshGEdge_H
#define	dtMeshGEdge_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtMesh1DOperator.h"

namespace dtOO {
  class dtGmshEdge;
  
  class dtMeshGEdge : public dtMesh1DOperator {
  public:
    dt__class(dtMeshGEdge, dtMeshOperator);     
    dtMeshGEdge();
    dtMeshGEdge(const dtMeshGEdge& orig);
    virtual ~dtMeshGEdge();
    virtual void operator()( dtGmshEdge * dtge );    
  };
}
#endif	/* dtMeshGEdge_H */

