#ifndef dtMeshGVertex_H
#define	dtMeshGVertex_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtMesh0DOperator.h"

namespace dtOO {
  class dtGmshVertex;
  
  class dtMeshGVertex : public dtMesh0DOperator {
    public:
      dt__class(dtMeshGVertex, dtMeshOperator);     
      dt__classSelfCreate(dtMeshGVertex);
      dtMeshGVertex();
      dtMeshGVertex(const dtMeshGVertex& orig);
      virtual ~dtMeshGVertex();
      virtual void operator()( dtGmshVertex * dtgv );    
    private:
      static bool _registrated;
  };
}
#endif	/* dtMeshGVertex_H */

