/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

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
