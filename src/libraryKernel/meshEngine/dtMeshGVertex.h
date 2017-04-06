#ifndef dtMeshGVertex_H
#define	dtMeshGVertex_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtMesh0DOperator.h"

namespace dtOO {
  class dtGmshVertex;
  
  class dtMeshGVertex : public dtMesh0DOperator {
  public:
    dt__class(dtMeshGVertex, dtMeshOperator);     
    dtMeshGVertex();
    dtMeshGVertex(const dtMeshGVertex& orig);
    virtual ~dtMeshGVertex();
    virtual void operator()( dtGmshVertex * dtgv );    
  };
}
#endif	/* dtMeshGVertex_H */

