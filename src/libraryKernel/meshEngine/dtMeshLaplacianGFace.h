#ifndef dtMeshLaplacianGFace_H
#define	dtMeshLaplacianGFace_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtMeshTransfinite2DOperator.h"

namespace dtOO {
  class dtGmshFace;
  
  class dtMeshLaplacianGFace : public dtMeshTransfinite2DOperator {
    public:
      dt__class(dtMeshLaplacianGFace, dtMeshOperator);     
      dt__classSelfCreate(dtMeshLaplacianGFace);
      dtMeshLaplacianGFace();
      dtMeshLaplacianGFace(
        const dtMeshLaplacianGFace& orig
      );
      virtual ~dtMeshLaplacianGFace();
      void operator()( dtGmshFace * dtgr );
    private:
      static bool _registrated;
  };
}
#endif	/* dtMeshLaplacianGFace_H */

