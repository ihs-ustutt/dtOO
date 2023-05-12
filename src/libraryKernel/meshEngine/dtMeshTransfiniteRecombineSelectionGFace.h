#ifndef dtMeshTransfiniteRecombineSelectionGFace_H
#define	dtMeshTransfiniteRecombineSelectionGFace_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtMeshGFace.h"

namespace dtOO {
  class dtGmshFace;
  
  class dtMeshTransfiniteRecombineSelectionGFace : public dtMeshGFace {
    public:
      dt__class(dtMeshTransfiniteRecombineSelectionGFace, dtMeshOperator);     
      dt__classSelfCreate(dtMeshTransfiniteRecombineSelectionGFace);
      dtMeshTransfiniteRecombineSelectionGFace();
      dtMeshTransfiniteRecombineSelectionGFace(
        const dtMeshTransfiniteRecombineSelectionGFace& orig
      );
      void init(
        ::QDomElement const & element,
        baseContainer const * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        lvH_dtMeshOperator const * const mO    
      );    
      virtual ~dtMeshTransfiniteRecombineSelectionGFace();
      void operator()( dtGmshFace * dtgr );    
    private:
      std::map< std::pair< int, dtInt >, bool > _recombineRule;
      dtInt _transfiniteArrangement;
    private:
      static bool _registrated;
  };
}
#endif	/* dtMeshTransfiniteRecombineSelectionGFace_H */

