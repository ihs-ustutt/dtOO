#ifndef dtMeshGFace_H
#define	dtMeshGFace_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtMesh2DOperator.h"

namespace dtOO {
  class dtGmshFace;
  
  class dtMeshGFace : public dtMesh2DOperator {
    public:
      dt__class(dtMeshGFace, dtMeshOperator);     
      dt__classSelfCreate(dtMeshGFace);
      dtMeshGFace();
      dtMeshGFace(const dtMeshGFace& orig);
      virtual ~dtMeshGFace();
      void operator()( dtGmshFace * dtgr );    
    private:
      static bool _registrated;
  };
}
#endif	/* dtMeshGFace_H */

