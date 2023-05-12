#ifndef dtMeshAndOrientGEdge_H
#define	dtMeshAndOrientGEdge_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtMeshGEdge.h"

namespace dtOO {
  class dtGmshEdge;
  
  class dtMeshAndOrientGEdge : public dtMeshGEdge {
    public:
      dt__class(dtMeshAndOrientGEdge, dtMeshOperator);     
      dt__classSelfCreate(dtMeshAndOrientGEdge);
      dtMeshAndOrientGEdge();
      dtMeshAndOrientGEdge(const dtMeshAndOrientGEdge& orig);
      virtual ~dtMeshAndOrientGEdge();
      virtual void operator()( dtGmshEdge * dtge );    
    private:
      static bool _registrated;
  };
}
#endif	/* dtMeshAndOrientGEdge_H */

