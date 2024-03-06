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
