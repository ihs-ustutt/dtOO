#ifndef dtMeshAndOrientGFace_H
#define	dtMeshAndOrientGFace_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtMeshGFace.h"

namespace dtOO {
  class dtGmshFace;
  
  class dtMeshAndOrientGFace : public dtMeshGFace {
    public:
      dt__class(dtMeshAndOrientGFace, dtMeshOperator);     
      dt__classSelfCreate(dtMeshAndOrientGFace);
      dtMeshAndOrientGFace();
      dtMeshAndOrientGFace(const dtMeshAndOrientGFace& orig);
      virtual ~dtMeshAndOrientGFace();
      void operator()( dtGmshFace * dtgr );    
    private:
      static bool _registrated;
  };
}
#endif	/* dtMeshAndOrientGFace_H */

