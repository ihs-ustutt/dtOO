#ifndef dtMeshGEdge_H
#define	dtMeshGEdge_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtMesh1DOperator.h"

namespace dtOO {
  class dtGmshEdge;
  
  class dtMeshGEdge : public dtMesh1DOperator {
    public:
      dt__class(dtMeshGEdge, dtMeshOperator);
      dt__classSelfCreate(dtMeshGEdge);
      dtMeshGEdge();
      dtMeshGEdge(const dtMeshGEdge& orig);
      virtual ~dtMeshGEdge();
      virtual void operator()( dtGmshEdge * dtge );    
    private:
      static bool _registrated;
  };
}
#endif	/* dtMeshGEdge_H */

