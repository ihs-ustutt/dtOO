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
