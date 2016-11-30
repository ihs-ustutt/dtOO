#ifndef dtMeshTransfiniteRecombineSelectionGFace_H
#define	dtMeshTransfiniteRecombineSelectionGFace_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtMeshGFace.h"

namespace dtOO {
  class dtGmshFace;
  
  class dtMeshTransfiniteRecombineSelectionGFace : public dtMeshGFace {
    public:
      dt__class(dtMeshTransfiniteRecombineSelectionGFace, dtMeshOperator);     
      dtMeshTransfiniteRecombineSelectionGFace();
      dtMeshTransfiniteRecombineSelectionGFace(
        const dtMeshTransfiniteRecombineSelectionGFace& orig
      );
      void init(
        ::QDomElement const & element,
        baseContainer const * const bC,
        cVPtrVec const * const cV,
        aFPtrVec const * const aF,
        aGPtrVec const * const aG,
        bVPtrVec const * const bV,
        labeledVectorHandling< dtMeshOperator * > const * const mO    
      );    
      virtual ~dtMeshTransfiniteRecombineSelectionGFace();
      void operator()( dtGmshFace * dtgr );    
    private:
      std::map< std::pair< int, int >, bool > _recombineRule;
      int _transfiniteArrangement;
  };
}
#endif	/* dtMeshTransfiniteRecombineSelectionGFace_H */

