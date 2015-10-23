#ifndef dtMeshTransfiniteGFace_H
#define	dtMeshTransfiniteGFace_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtMesh2DOperator.h"

class GFace;
class MVertex;

namespace dtOO {
  class dtGmshFace;
  
  class dtMeshTransfiniteGFace : public dtMesh2DOperator {
    public:
      dt__class(dtMeshTransfiniteGFace, dtMeshOperator);     
      dtMeshTransfiniteGFace();
      dtMeshTransfiniteGFace(
        const dtMeshTransfiniteGFace& orig
      );
      virtual ~dtMeshTransfiniteGFace();
      void operator()( dtGmshFace * dtgr );    
    private:
    static void computeEdgeLoops(
      const ::GFace *gf, 
      std::vector< ::MVertex * > & all_mvertices, 
      std::vector< int > & indices
    );  
  };
}
#endif	/* dtMeshTransfiniteGFace_H */

